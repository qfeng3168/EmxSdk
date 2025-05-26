#!/usr/bin/python3
import os
import sys
import json
import utils
import datetime
from build import build

utils.print_info("./release.py 0/1")

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

if len(sys.argv) == 2:
    if sys.argv[1] == '1':
        build(env.top, env.firmware + "/app", False, True)
    else:
        build(env.top, env.firmware + "/app", False, False)
else:
    build(env.top, env.firmware + "/app", False, True)

with open(env.firmware + "/configs/devInfo.json", 'w+') as f:
    json.dump(dev_info, f, indent=2, ensure_ascii=False)
f.close()

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
md5list=json.loads("[]")
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
    md5list.append(utils.Md5.file(dst_rom_file))
dev_info_with_md5 = dev_info
dev_info_with_md5["md5"] = md5list
# 将devInfo.json写入到output固件发布目录
with open(output + "/devInfo.json", 'w+') as f:
    json.dump(dev_info_with_md5, f, indent=2, ensure_ascii=False)
f.close()


# 常规打包(nor、nand)用于烧片的裸flash文件
utils.flash(env.imgs, env.psp["partition"], output + "/ep_flash_sd.bin")
#nand flash 分区表制作
# utils.partition_nand(env.imgs, env.psp["partition"], output + "/" + firmware_version)
os.chdir(env.imgs)
#拷贝boot.bin 或者fip.bin 到固件包，用于设备裸片启动或烧录
os.system("cp {} {}".format("fip.bin", output))
os.system("cp {} {}".format("boot.bin", output))
os.chdir(env.top)
#emmc 打包实例
# utils.flash(env.imgs, env.psp["partition"], output + "/flash_partition_USER.bin")
# os.chdir(env.imgs)
# os.system("cp {} {}".format("fip.bin", output))
# os.system("cp {} {}".format("boot.bin", output + "/flash_partition_BOOT1.bin"))
# os.system("cp {} {}".format("*.exe", output))
# os.chdir(env.top)

