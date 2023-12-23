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

def exec_dir(cross, path):
    if len(os.listdir(path)):
        os.system("chmod +x {}/*")
        if cross != "x86_64":
            os.system("{}-strip {}/*".format(cross, path))

def pack_3dr(env):
    print(f"pack_3dr")
    for item in env.product["custom"]["3drDepend"]:
        if item["name"] == "curl":
            src = env.Emx3dr + "/common/{}/{}/{}/{}/{}".format(item["name"], item["version"], item["ssl"], item["ssl_version"], env.cross)
            dst = env.firmware + "/3dr"
            copy_file_list(src + "/lib", dst + "/lib", item["shared"])
            copy_file_list(src + "/bin", dst + "/bin", item["bin"])
        else:
            src = env.Emx3dr + "/common/{}/{}/{}".format(item["name"], item["version"], env.cross)
            dst = env.firmware + "/3dr"
            copy_file_list(src + "/lib", dst + "/lib", item["shared"])
            copy_file_list(src + "/bin", dst + "/bin", item["bin"])

def strip_3dr(env):
    print(f"strip_3dr")
    exec_dir(env.cross, env.firmware + "/3dr/bin")
    exec_dir(env.cross, env.firmware + "/3dr/lib")

def pack_file(src_top, dst_top, remap):
    print(f"pack_file")
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

def custom(env):
    print(f"custom")
    pack_3dr(env)
    strip_3dr(env)
    pack_file(env.top, env.top, env.product["custom"]["fileDepend"])

if __name__ == "__main__":
    env0 = utils.Env(os.getcwd() + "/..")
    print(f"custom main")
    custom(env0)
