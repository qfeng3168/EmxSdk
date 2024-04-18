#!/usr/bin/python3
import os
import sys
import json
from utils import *


def verify_dependency(profile, version):
    with open("profile/" + profile + "/profile.json") as f:
        profile_cfg = json.load(f)
        f.close()
    depend_path = "Public/Emx3dr/chip/{}/{}/ai/{}".format(
        profile_cfg["ChipManufactory"], profile_cfg["ChipSerial"], profile_cfg["ChipAiVersion"])
    for lib in profile_cfg["runtime"]["lib"]:
        path = depend_path+"/" + lib
        if not os.path.exists(path):
            print(f"could not found {path}")
            exit(-1)
    for file in profile_cfg["runtime"]["file"]:
        path = depend_path+"/" + file
        if not os.path.exists(depend_path+"/" + file):
            print(f"could not found {path}")
            exit(-1)

    AiPluginJson = {}
    AiPluginJson["profile"] = profile
    AiPluginJson["version"] = version
    AiPluginJson["verify"] = False

    # 拷贝3dr中的lib和file
    AiPluginLibsJson = []
    # 遍历AiPlugin所有文件并生成md5
    AiPluginMd5Json = []
    for lib in profile_cfg["runtime"]["lib"]:
        AiPluginLibsJson.append(lib)
        abs_path = os.path.join(depend_path, lib)
        rel_path = os.path.relpath(abs_path, depend_path)
        item = {}
        item["file"] = rel_path
        item["md5"] = utils.Md5.file(abs_path)
        AiPluginMd5Json.append(item)
    AiPluginJson["libs"] = AiPluginLibsJson

    for file in profile["runtime"]["file"]:
        abs_path = os.path.join(depend_path, file)
        rel_path = os.path.relpath(abs_path, depend_path)
        item = {}
        item["file"] = rel_path
        item["md5"] = utils.Md5.file(abs_path)
        AiPluginMd5Json.append(item)

    AiPluginJson["files"] = AiPluginMd5Json
    print_err("%s",os.getcwd() + "/output/EmxAiPlugin-Debug/version/AiPlugin.json")
    with open(os.getcwd() + "/output/EmxAiPlugin-Debug/version/AiPlugin.json", "w+") as f:
        json.dump(AiPluginJson, f, indent=4)

def build(install_dir, profile, version):
    if not os.path.exists("profile/" + profile):
        print_err("profile {} not exist".format(profile))
        exit(-1)

    tmp_build = ".tmp_build"
    if os.path.exists(tmp_build):
        os.system("rm -r " + tmp_build)
    os.system("mkdir " + tmp_build)
    os.chdir(tmp_build)
    cmd_check(
        "cmake .. "
        "-DEmxAiPluginVersion={}"
        " -DAiPluginProfile={}"
        " -DInstallDir={}".format(version, profile, install_dir))
    cmd_check("make -j install")
    os.chdir("..")
    os.system("rm -r " + tmp_build)
    verify_dependency(profile, version)


if __name__ == "__main__":
    os.chdir("..")
    profile_list = []
    for p in os.listdir("profile"):
        profile_list.append(p)
    if len(sys.argv) != 2:
        print_err("usage:./build.py profile_index")
        print_err("eg. ./build.py 0")
        print_info("-------------------profile list-------------------")
        i = 0
        for p in profile_list:
            print_info(str(i) + ":" + p)
            i += 1
        exit(0)

    build(os.getcwd() + "/output/EmxAiPlugin-Debug",
          profile_list[int(sys.argv[1])], "Debug")
