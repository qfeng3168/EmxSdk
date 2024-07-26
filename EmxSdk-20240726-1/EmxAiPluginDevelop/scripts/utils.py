#!/usr/bin/python3
import hashlib
import os


def print_err(string):
    print("\033[1;31m" + string + "\033[0m")


def print_info(string):
    print("\033[1;35m" + string + "\033[0m")


def cmd_check(cmd_str):
    ret = os.system(cmd_str)
    if ret >> 8 != 0:
        print_err("error")
        exit()


def get_file_md5(file_path):
    """  
    计算文件的MD5哈希值  
    :param file_path: 文件的路径  
    :return: 文件的MD5哈希值  
    """
    # 创建一个md5对象
    md5_hash = hashlib.md5()

    # 确保文件存在
    if not os.path.isfile(file_path):
        return None

    # 打开文件，读取内容，并更新md5对象
    with open(file_path, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            md5_hash.update(chunk)

    # 获取16进制表示的哈希值
    return md5_hash.hexdigest()
