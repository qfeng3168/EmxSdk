#!/usr/bin/python3
import os
import sys
from utils import *


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
        "-DEmxMediaVersion={}"
        " -DProfile={}"
        " -DInstallDir={}".format(version, profile, install_dir))
    cmd_check("make -j32 install")
    os.chdir("..")
    os.system("rm -r " + tmp_build)


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

    build(os.getcwd() + "/output/EmxMedia-Debug", profile_list[int(sys.argv[1])], "Debug")
