#!/usr/bin/python3
import os
import utils


def build(top, install_dir, spare):
    os.chdir(top)
    tmp_build = ".tmp_build"
    os.system("mkdir -p " + tmp_build)
    os.chdir(tmp_build)
    os.system("rm -rf *")
    if spare:
        spare_define = "-DEMX_SPARE=1"
    else:
        spare_define = ""
    if len(install_dir):
        install_define = "-DInstallDir={}".format(install_dir)
    else:
        install_define = ""
    os.system("cmake .. {} {}".format(install_define, spare_define))
    os.system("make -j16 install")
    os.chdir(top)
    os.system("rm -rf " + tmp_build)
    utils.print_info("Build done.")


if __name__ == "__main__":
    build(os.getcwd() + "/..", "", False)
