#!/usr/bin/python3
import os
import sys
from utils import *
from build import build
import datetime

os.chdir("..")
version_file = "EmxAiPluginVersion"
# read old version
with open(version_file, 'r') as f:
    old_version = f.read()
    f.close()

# split date and build version
old_date, build_ver = old_version.split("-")
# generate new version
date = datetime.datetime.now().strftime('%Y%m%d')
if old_date != date:
    build_ver = "0"
version = date + '-' + str(int(build_ver) + 1)

# write back
with open(version_file, 'w+') as f:
    f.write(version)
    f.close()

install_dir = os.getcwd() + "/output/EmxAiPlugin-" + version

for p in os.listdir("profile"):
    build(install_dir, p, version)

print_info("EmxAiPlugin-{} release done".format(version))
