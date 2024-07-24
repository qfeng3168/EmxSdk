#!/usr/bin/python3
import os
import json
import utils
import crypt


def chmod_dir(path):
    if not os.path.exists(path):
        return
    os.system("chmod +x {}/*".format(path))
    for e in os.listdir(path):
        dst_path = os.path.join(path, e)
        if os.path.isdir(dst_path):
            # 如果是目录就递归扫描
            chmod_dir(dst_path)


def exec_dir(cross, path):
    if not os.path.exists(path):
        return
    os.system("chmod +x {}/*".format(path))
    if cross != "x86_64":
        os.system("{}-strip {}/*".format(cross, path))
    for e in os.listdir(path):
        dst_path = os.path.join(path, e)
        if os.path.isdir(dst_path):
            # 如果是目录就递归扫描
            exec_dir(cross, dst_path)


def copy_file_list(src, dst, file_list):
    if len(file_list) == 0:
        return
    if not os.path.exists(src):
        utils.print_err("{} not exists".format(src))
    utils.try_mkdir(dst)
    for file in file_list:
        utils.copy(os.path.join(src, file), dst)


def remap_copy(src_top, dst_top, remap):
    for item in remap:
        src = os.path.join(src_top, item["src"])
        dst = os.path.join(dst_top, item["dst"])
        if not os.path.exists(src):
            utils.print_err(src + "is not exists")
            exit(-1)
        if os.path.isdir(src):
            os.system("mkdir -p " + dst)
            utils.copy(src + "/*", dst)
        else:
            parent_dir = dst.split(dst.split("/")[-1])[0]
            os.system("mkdir -p " + parent_dir)
            utils.copy(src, dst)


def build_param(env):
    utils.copy_tree(env.top + "/orgParam", env.firmware + "/param")
    utils.copy(env.top + "/psp/param", env.firmware)
    param_codec = utils.ParamCodec(env.cipher["key"], env.cipher["iv"])
    param_codec.encode(env.firmware + "/param", env.firmware + "/param")


def build_configs(env):
    utils.copy_tree(env.top + "/orgConfigs", env.firmware + "/configs")
    utils.copy(env.top + "/psp/configs", env.firmware)
    chmod_dir(env.firmware + "/configs")
    # 将密钥加密保存到firmware/configs中
    cipher = utils.AesKey.encrypt(list(env.cipher["key"].encode(encoding="utf-8")),
                                  list(env.cipher["iv"].encode(encoding="utf-8")))
    with open(env.firmware + "/configs/cipher", 'wb+') as f:
        f.write(cipher)
        f.close()


def build_ai_plugin(env):
    utils.copy(env.top + "/psp/AiPlugin", env.firmware+"/media")
    AiPluginDir = f"{env.firmware}/media/AiPlugin"
    profile_dir = "{}/{}/{}".format(env.EmxAiPlugin,
                                    env.psp["EmxAiPluginVersion"], env.psp["EmxAiPluginProfile"])
    utils.copy(f"{profile_dir}/libEmxAiPlugin.so",
               f"{env.firmware}/media/AiPlugin")
    with open(f"{profile_dir}/profile.json") as f:
        profile = json.load(f)
        f.close()
    AiPluginJson = {}
    AiPluginJson["profile"] = env.psp["EmxAiPluginProfile"]
    AiPluginJson["version"] = env.psp["EmxAiPluginVersion"]
    AiPluginJson["verify"] = True
    runtime_dir = f"{env.Emx3dr}/chip/{profile['ChipManufactory']}/{profile['ChipSerial']}/ai/{profile['ChipAiVersion']}"

    # 拷贝3dr中的lib和file
    AiPluginLibsJson = []
    for lib in profile["runtime"]["lib"]:
        AiPluginLibsJson.append(lib)
        utils.copy(f"{runtime_dir}/{lib}", AiPluginDir)
    AiPluginJson["libs"] = AiPluginLibsJson

    for file in profile["runtime"]["file"]:
        utils.copy(f"{runtime_dir}/{file}", AiPluginDir)

    # 遍历AiPlugin所有文件并生成md5
    AiPluginMd5Json = []
    for root, dirs, files in os.walk(AiPluginDir):
        for file in files:
            abs_path = os.path.join(root, file)
            rel_path = os.path.relpath(abs_path, AiPluginDir)
            item = {}
            item["file"] = rel_path
            item["md5"] = utils.Md5.file(abs_path)
            AiPluginMd5Json.append(item)
    AiPluginJson["files"] = AiPluginMd5Json
    with open(AiPluginDir + "/AiPlugin.json", "w+") as f:
        json.dump(AiPluginJson, f, indent=4)


def build_media(env):
    utils.try_mkdir(env.firmware + "/media")
    profile = env.EmxMedia + \
        "/{}/{}".format(env.psp["EmxMediaVersion"], env.psp["EmxMediaProfile"])
    EmxMediaConfigs = ''
    if(len(env.psp.get('EmxMediaConfigs', [])) !=0):
        EmxMediaConfigs = profile + "/{}".format(env.psp["EmxMediaConfigs"])
    else:
        EmxMediaConfigs = profile + "/configs.json"
    with open(EmxMediaConfigs) as f:
        configs = json.load(f)
        f.close()
    if configs["EmxSdkVersion"] != env.sdk_version:
        utils.print_err(
            "Warn:the EmxSdkVersion of EmxMedia depended is not match to that in psp.json")
    chip_sdk = env.Emx3dr + "/chip/{}/{}/{}".format(
        configs["ChipManufactory"],
        configs["ChipSerial"],
        configs["ChipSdkVersion"]
    )
    media_lib = env.firmware + "/media/lib"
    utils.try_mkdir(media_lib)
    copy_file_list(chip_sdk + "/lib", media_lib, configs["runtime"]["lib"])
    exec_dir(env.cross, media_lib)

    media_drivers = env.firmware + "/media/drivers"
    EmxMediaDrivers = ''
    if(len(env.psp.get('EmxMediaDrivers', [])) !=0):
        EmxMediaDrivers = chip_sdk + "/{}".format(env.psp["EmxMediaDrivers"])
    else:
        EmxMediaDrivers = chip_sdk + "/drivers"  
    utils.try_mkdir(media_drivers)
    copy_file_list(EmxMediaDrivers, media_drivers,
                   configs["runtime"]["drivers"])

    media_bin = env.firmware + "/media/bin"
    utils.try_mkdir(media_bin)
    utils.copy("{}/EmxMediaServer".format(profile), media_bin)
    exec_dir(env.cross, media_bin)


def build_app(env):
    app = env.firmware + "/app"
    utils.try_mkdir(app)
    app_drivers = env.firmware + "/app/drivers"
    utils.try_mkdir(app_drivers)
    utils.copy(
        env.EmxSdk + "/{}/cross/{}/bin".format(env.sdk_version, env.cross), app)
    exec_dir(env.cross, app + "/bin")
    utils.copy(
        env.EmxSdk + "/{}/cross/{}/lib".format(env.sdk_version, env.cross), app)
    exec_dir(env.cross, app + "/lib")


def build_3dr(env):
    def load_depend(depend_file_path):
        with open(depend_file_path) as f:
            configs = json.load(f)
            f.close()
        for item in configs["depend"]:
            src = env.Emx3dr + \
                "/common/{}/{}/{}".format(item["name"],
                                          item["version"], env.cross)
            dst = env.firmware + "/3dr"
            copy_file_list(src + "/lib", dst + "/lib", item["shared"])
            copy_file_list(src + "/bin", dst + "/bin", item["bin"])

    utils.try_mkdir(env.firmware + "/3dr/bin")
    utils.try_mkdir(env.firmware + "/3dr/lib")
    load_depend(
        env.EmxSdk + "/{}/configs/EmxCoreCfg.json".format(env.sdk_version))
    load_depend(
        env.EmxSdk + "/{}/configs/EmxMediaCfg.json".format(env.sdk_version))
    exec_dir(env.cross, env.firmware + "/3dr/bin")
    exec_dir(env.cross, env.firmware + "/3dr/lib")


def build_scripts(env):
    utils.copy(env.top + "/psp/scripts", env.firmware)
    chmod_dir(env.firmware + "/scripts")


def firmware(env):
    utils.remkdir(env.firmware)
    build_3dr(env)
    build_app(env)
    build_media(env)
    build_ai_plugin(env)
    build_configs(env)
    build_param(env)
    build_scripts(env)
    remap_copy(env.EmxRes, env.firmware, env.product["EmxResToFirmware"])


def spare(env):
    if not env.product["spare"]["ena"]:
        return
    utils.try_rmdir(env.spare)
    utils.copy(env.firmware, env.spare)
    # 刪除app目录下的部分无用文件
    for file in env.product["spare"]["excludeFileList"]:
        file_path = os.path.join(env.spare, file)
        if os.path.exists(file_path) or file_path[-1] == "*":
            os.system("rm -r " + file_path)


def generate_password_hash(password):
    salt = crypt.mksalt(crypt.METHOD_SHA512)
    hashed_password = crypt.crypt(password, salt)
    return hashed_password


def product_shadow(env):
    etc = env.firmware + "/configs/etc"
    utils.try_mkdir(etc)
    shadow_file = etc + "/shadow"
    utils.try_rmfile(shadow_file)
    passwd_file = etc + "/passwd"
    utils.try_rmfile(passwd_file)
    group_file = etc + "/group"
    utils.try_rmfile(group_file)

    for item in env.product["user"]:
        name = item["name"]
        uid = item["uid"]
        gid = item["gid"]
        comment = item["comment"]
        home_directory = item["home_directory"]
        src_password = item["password"]
        dst_password = generate_password_hash(src_password)
        print("name:", name)
        print("src password:", src_password)
        print("dst password:", dst_password)
        # 写入 /etc/passwd
        with open(passwd_file, 'a', encoding="utf-8") as file:
            file.write(
                f"{name}:{dst_password}:{uid}:{gid}:{comment}:{home_directory}:/bin/sh\n")
        # 写入 /etc/group
        with open(group_file, 'a', encoding="utf-8") as file:
            file.write(f"{name}:x:{gid}:\n")
        # 写入 /etc/shadow
        with open(shadow_file, 'a', encoding="utf-8") as file:
            file.write(f"{name}:{dst_password}\n")


def product(env):
    firmware(env)
    spare(env)
    remap_copy(env.EmxRes, env.top, env.psp["EmxResToProduct"])
    custom_file = env.top + "/scripts/custom.py"
    if os.path.exists(custom_file):
        os.system(custom_file)
    else:
        print(f"{custom_file} not found.")
    product_shadow(env)


if __name__ == "__main__":
    env0 = utils.Env(os.getcwd() + "/..")
    product(env0)
