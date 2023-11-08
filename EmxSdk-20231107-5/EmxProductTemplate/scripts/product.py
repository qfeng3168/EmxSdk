#!/usr/bin/python3
import os
import json
import utils


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
    utils.copy(env.top + "/param", env.firmware)
    utils.copy(env.top + "/psp/param", env.firmware)
    param_codec = utils.ParamCodec(env.cipher["key"], env.cipher["iv"])
    param_codec.encode(env.firmware + "/param", env.firmware + "/param")


def build_configs(env):
    utils.copy(env.top + "/configs", env.firmware)
    utils.copy(env.top + "/psp/configs", env.firmware)
    # 将密钥加密保存到firmware/configs中
    cipher = utils.AesKey.encrypt(list(env.cipher["key"].encode(encoding="utf-8")),
                                  list(env.cipher["iv"].encode(encoding="utf-8")))
    with open(env.firmware + "/configs/cipher", 'wb+') as f:
        f.write(cipher)
        f.close()


def build_media(env):
    utils.try_mkdir(env.firmware + "/media")
    profile = env.EmxMedia + "/{}/{}".format(env.psp["EmxMediaVersion"], env.psp["EmxMediaProfile"])
    with open(profile + "/configs.json") as f:
        configs = json.load(f)
        f.close()
    if configs["EmxSdkVersion"] != env.sdk_version:
        utils.print_err("Warn:the EmxSdkVersion of EmxMedia depended is not match to that in psp.json")
    chip_sdk = env.Emx3dr + "/chip/{}/{}/{}".format(
        configs["ChipManufactory"],
        configs["ChipSerial"],
        configs["ChipSdkVersion"]
    )
    media_lib = env.firmware + "/media/lib"
    utils.try_mkdir(media_lib)
    copy_file_list(chip_sdk + "/lib", media_lib, configs["runtime"]["lib"])
    os.system("chmod +x " + media_lib + "/*")

    media_drivers = env.firmware + "/media/drivers"
    utils.try_mkdir(media_drivers)
    copy_file_list(chip_sdk + "/drivers", media_drivers, configs["runtime"]["drivers"])

    media_bin = env.firmware + "/media/bin"
    utils.try_mkdir(media_bin)
    utils.copy("{}/EmxMediaServer".format(profile), media_bin)
    os.system("chmod +x " + media_bin + "/*")


def build_app(env):
    app = env.firmware + "/app"
    utils.try_mkdir(app)
    utils.copy(env.EmxSdk + "/{}/cross/{}/bin".format(env.sdk_version, env.cross), app)
    os.system("chmod +x " + app + "/bin/*")
    utils.copy(env.EmxSdk + "/{}/cross/{}/lib".format(env.sdk_version, env.cross), app)
    os.system("chmod +x " + app + "/lib/*")


def build_3dr(env):
    def load_depend(depend_file_path):
        with open(depend_file_path) as f:
            configs = json.load(f)
            f.close()
        for item in configs["depend"]:
            src = env.Emx3dr + "/common/{}/{}/{}".format(item["name"], item["version"], env.cross)
            dst = env.firmware + "/3dr"
            copy_file_list(src + "/lib", dst + "/lib", item["shared"])
            copy_file_list(src + "/bin", dst + "/bin", item["bin"])

    utils.try_mkdir(env.firmware + "/3dr/bin")
    utils.try_mkdir(env.firmware + "/3dr/lib")
    load_depend(env.EmxSdk + "/{}/configs/EmxCoreCfg.json".format(env.sdk_version))
    load_depend(env.EmxSdk + "/{}/configs/EmxMediaCfg.json".format(env.sdk_version))
    os.system("chmod +x " + env.firmware + "/3dr/bin/*")
    os.system("chmod +x " + env.firmware + "/3dr/lib/*")


def build_scripts(env):
    utils.copy(env.top + "/psp/scripts", env.firmware)
    os.system("chmod +x " + env.firmware + "/scripts/*")


def firmware(env):
    utils.remkdir(env.firmware)
    build_3dr(env)
    build_app(env)
    build_media(env)
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


def product(env):
    firmware(env)
    spare(env)
    remap_copy(env.EmxRes, env.top, env.psp["EmxResToProduct"])
    remap_copy(env.top, env.top, env.product["ProductAppendFile"])


if __name__ == "__main__":
    env0 = utils.Env(os.getcwd() + "/..")
    product(env0)
