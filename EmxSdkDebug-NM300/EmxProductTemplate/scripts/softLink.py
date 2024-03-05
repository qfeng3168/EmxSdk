#!/usr/bin/python3
import os
import sys
import utils

# 创建本地软连接便于开发

if __name__ != "__main__":
    exit(-1)

if len(sys.argv) != 5:
    utils.print_err(
        "usage: ./softLink.py the/path/of/EmxSdk the/path/of/Emx3dr the/path/of/EmxRes the/path/of/EmxMedia")
    exit(-1)

cwd = os.getcwd()
top = cwd + "/.."
for i in range(1, len(sys.argv)):
    s = sys.argv[i]
    if len(s) and not os.path.isabs(s):
        s = os.path.join(cwd, s)
    if not os.path.exists(s):
        utils.print_err(s + " not exist")
        exit(-1)
    link = os.path.join(top, s.split("/")[-1])
    if os.path.islink(link):
        os.system("rm -r " + link)
    os.system("ln -s {} {}".format(s, link))
