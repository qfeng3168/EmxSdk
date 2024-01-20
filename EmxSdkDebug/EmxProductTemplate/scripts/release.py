#!/usr/bin/python3
import os
import json
import utils
import datetime
from build import build

os.chdir("..")
env = utils.Env(os.getcwd())

# 获取并更新版本号
dev_info = env.product["devInfo"]
main_ver, sub_ver, old_date, build_ver = dev_info["inner"]["version"].split(".")
date = datetime.datetime.now().strftime('%Y%m%d')
if old_date != date:
    build_ver = "0"
inner_version = main_ver + '.' + sub_ver + '.' + date + '.' + str(int(build_ver) + 1)
dev_info["inner"]["version"] = inner_version
with open(env.top + "/product.json", 'w+') as f:
    json.dump(env.product, f, indent=2, ensure_ascii=False)
f.close()

# 构建应用程序安装到firmware/app
utils.print_info("build for firmware")
build(env.top, env.firmware + "/app", False)
with open(env.firmware + "/configs/devInfo.json", 'w+') as f:
    json.dump(dev_info, f, indent=2, ensure_ascii=False)
f.close()

if not env.product["spare"]["ena"]:
    command = 'ubinize -o {} -m 2048 -p 128KiB  firmwareUbinize.cfg'.format(env.imgs+"/firmware.bin")
    subprocess.call(command,shell=True)

# 构建应用程序安装到spare/app
if env.product["spare"]["ena"]:
    utils.print_info("build for spare")
    build(env.top, env.spare + "/app", True)
    with open(env.spare + "/configs/devInfo.json", 'w+') as f:
        json.dump(dev_info, f, indent=2, ensure_ascii=False)
    f.close()

os.chdir(env.imgs)
os.system("chmod +x mkImg.py")
os.system("./mkImg.py")
os.chdir(env.top)

# firmware version
firmware_version = env.product["devInfo"]["outer"]["type"]
firmware_version += "_" + env.product["devInfo"]["inner"]["version"]
firmware_version += "_" + env.product["devInfo"]["outer"]["version"]
output = env.top + "/output/" + firmware_version
os.system("mkdir -p " + output)

# 打包rom
for rom in env.product["rom"]:
    dst_rom_file = output + "/" + firmware_version + "_" + rom["name"] + ".bin"
    utils.rom(env.product["cipher"]["key"],
              env.product["cipher"]["iv"],
              env.product["devInfo"],
              dst_rom_file,
              env.imgs,
              env.psp["partition"],
              rom["imgs"],
              rom["pre_script"],
              rom["post_script"])

# 打包用于烧片的裸flash文件
utils.flash(env.imgs, env.psp["partition"], output + "/" + firmware_version + "_RawFlash.bin")
