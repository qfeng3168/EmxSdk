#!/usr/bin/python3
import os
import sys
import utils

# 创建本地软连接便于开发

if __name__ != "__main__":
    exit(-1)

if len(sys.argv) != 2:
    utils.print_err(
        "usage: ./softLink.py the/path/of/Public")
    exit(-1)

cwd = os.getcwd()
top = cwd + "/.."

if len(sys.argv[1]) and not os.path.isabs(sys.argv[1]):
    s = os.path.join(cwd, sys.argv[1])
if not os.path.exists(s):
    utils.print_err(s + " not exist")
    exit(-1)
link = os.path.join(top, s.split("/")[-1])
if os.path.islink(link):
    os.system("rm -r " + link)
os.system("ln -s {} {}".format(s, link))
