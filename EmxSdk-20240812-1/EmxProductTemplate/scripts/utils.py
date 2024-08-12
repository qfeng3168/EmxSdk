#!/usr/bin/python3

# note!
# sudo apt install pip
# pip3 install pycryptodome
from Crypto.Cipher import AES
import os
import hashlib
from ctypes import *
import base64
import json
import shutil


def print_err(string):
    print("\033[1;31m" + string + "\033[0m")


def print_info(string):
    print("\033[1;35m" + string + "\033[0m")


def cmd_check(cmd_str):
    ret = os.system(cmd_str)
    if ret >> 8 != 0:
        print_err("error")
        exit()


def try_mkdir(path):
    if not os.path.exists(path):
        os.system("mkdir -p " + path)


def try_rmdir(path):
    if os.path.exists(path):
        os.system("rm -r " + path)


def remkdir(path):
    try_rmdir(path)
    try_mkdir(path)


def try_rmfile(path):
    if os.path.exists(path):
        os.system("rm " + path)


def get_product_json(path):
    with open(path) as f_product:
        product = json.load(f_product)
        f_product.close()
    return product


def copy(src, dst):
    os.system("cp -R {} {}".format(src, dst))


def copy_tree(src, dst):
    shutil.copytree(src, dst, symlinks=True)


class Env:
    def __init__(self, top):
        self.top = top
        self.Public = top + "/Public"
        self.EmxSdk = self.Public + "/EmxSdk"
        self.Emx3dr = self.Public + "/Emx3dr"
        self.EmxRes = self.Public + "/EmxRes"
        self.EmxMedia = self.Public + "/EmxMedia"
        self.EmxAiPlugin = self.Public + "/EmxAiPlugin"
        self.rootfs = top + "/rootfs"
        self.firmware = top + "/firmware"
        self.spare = top + "/spare"
        self.imgs = top + "/imgs"
        self.param = top + "/param"
        self.product = get_product_json(top + "/product.json")
        self.psp = get_product_json(top + "/psp/psp.json")
        self.sdk_version = self.product["EmxSdkVersion"]
        self.cross = self.psp["Cross"]
        self.cipher = self.product["cipher"]


class Aes:
    """用于AES加解密"""

    def __init__(self, key, iv):
        key_bytes = bytes(key, encoding='utf-8')
        iv_bytes = bytes(iv, encoding='utf-8')
        self.aes = AES.new(key_bytes, AES.MODE_CBC, iv_bytes)

    def decode_file(self, src, dst):
        """
        解密一个文件
        :param src: 待解密文件路径
        :param dst: 解密后文件存储路径
        :return: none
        """
        with open(src, "rb") as f:
            data = f.read()
            f.close()
            data = self.decode(data)
            with open(dst, "wb+") as f:
                f.write(data)
                f.close()

    def encode_file(self, src, dst):
        """
        加密一个文件
        :param src: 待加密文件路径
        :param dst: 加密后文件存储路径
        :return: none
        """
        with open(src, "rb") as f:
            data = f.read()
            f.close()
            data = self.encode(data)
            with open(dst, "wb+") as f:
                f.write(data)
                f.close()

    def encode(self, content):
        """
        加密bytes
        :param content: 待加密bytes
        :return: 加密后bytes
        """
        # 处理明文
        content_padding = self.pkcs7_padding(content)
        # 加密
        encode_bytes = self.aes.encrypt(bytes(content_padding))
        return encode_bytes

    def decode(self, content):
        """
        解密bytes
        :param content: 待解密bytes
        :return: 解密后bytes
        """
        # 解密
        aes_decode_bytes = self.aes.decrypt(content)
        # 去除填充内容
        result = self.pkcs7_unpadding(aes_decode_bytes)
        return result

    @staticmethod
    def pkcs7_padding(data):
        bs = AES.block_size  # 16
        length = len(data)
        padding = bs - length % bs
        return data + bytes([padding] * padding)

    @staticmethod
    def pkcs7_unpadding(data):
        length = len(data)
        unpadding = data[length - 1]
        return data[0:length - unpadding]


class AesDir:
    """将src目录下所有文件和目录加密/解密存储到dst目录下"""

    def __init__(self, key, iv):
        self.key = key
        self.iv = iv

    def encode(self, src, dst):
        self._codec(src, dst, True)

    def decode(self, src, dst):
        self._codec(src, dst, False)

    def _codec(self, src, dst, codec_flag):
        if src != dst:
            os.system("rm -r " + dst)
        self._proc_dir(src, dst, codec_flag)

    def _proc_dir(self, src_dir, dst_dir, codec_flag):
        # print("src={}".format(src_dir))
        # print("dst={}".format(dst_dir))
        # 如果目标目录不存在就创建一个
        os.system("mkdir -p {}".format(dst_dir))
        for e in os.listdir(src_dir):
            src_path = os.path.join(src_dir, e)
            dst_path = os.path.join(dst_dir, e)
            if os.path.isdir(src_path):
                # 如果是目录就递归扫描
                self._proc_dir(src_path, dst_path, codec_flag)
            else:
                # print("src={}".format(src_path))
                # print("dst={}".format(dst_path))
                # print("\n")
                # 如果是文件就加密/解密处理
                aes = Aes(self.key, self.iv)
                if codec_flag:
                    aes.encode_file(src_path, dst_path)
                else:
                    aes.decode_file(src_path, dst_path)


class ParamCodec:
    """将src目录下所有文件和目录加密/解密存储到dst目录下"""

    def __init__(self, key, iv):
        self.key = key
        self.iv = iv

    def encode(self, src, dst):
        self._codec(src, dst, True)

    def decode(self, src, dst):
        self._codec(src, dst, False)

    def _codec(self, src, dst, codec_flag):
        if src != dst:
            os.system("rm -r " + dst)
        self._proc_dir(src, dst, codec_flag)

    def _proc_dir(self, src_dir, dst_dir, codec_flag):
        # print("src={}".format(src_dir))
        # print("dst={}".format(dst_dir))
        # 如果目标目录不存在就创建一个
        os.system("mkdir -p {}".format(dst_dir))
        for e in os.listdir(src_dir):
            src_path = os.path.join(src_dir, e)
            dst_path = os.path.join(dst_dir, e)
            if os.path.isdir(src_path):
                # 如果是目录就递归扫描
                self._proc_dir(src_path, dst_path, codec_flag)
            else:
                # print("src={}".format(src_path))
                # print("dst={}".format(dst_path))
                # print("\n")
                # 如果是文件就加密/解密处理
                aes = Aes(self.key, self.iv)
                if codec_flag:
                    with open(src_path, "rb") as fs:
                        data = fs.read()
                        fs.close()
                        data = aes.encode(data)
                        data = base64.encodebytes(data).replace(b"\n", b"")
                        with open(dst_path, "wb+") as fd:
                            fd.write(data)
                            fd.close()
                else:
                    with open(src_path, "rb") as fs:
                        data = fs.read()
                        fs.close()
                        data = base64.decodebytes(data)
                        data = aes.decode(data)
                        with open(dst_path, "wb+") as fd:
                            fd.write(data)
                            fd.close()


class Md5:
    @staticmethod
    def file(file):
        m = hashlib.md5()
        with open(file, 'rb') as f:
            content = f.read()
            m.update(content)
            return m.hexdigest()

    @staticmethod
    def content(content):
        m = hashlib.md5()
        m.update(content)
        return m.hexdigest()


def mx(z, y, sum1, k, p, e):
    return c_uint32(((z.value >> 5 ^ y.value << 2) + (y.value >> 3 ^ z.value << 4)) ^ (
        (sum1.value ^ y.value) + (k[(p & 3) ^ e.value] ^ z.value)))


class XXTEA:
    @staticmethod
    def encrypt(v, n, k=None, delta=0x933779b9):
        if k is None:
            k = [0x45, 0x41, 0x50, 0x494C]
        sum1 = c_uint32(0)
        z = c_uint32(v[n - 1])
        rounds = 6 + 52 // n
        e = c_uint32(0)

        while rounds > 0:
            sum1.value += delta
            e.value = ((sum1.value >> 2) & 3)  # e都要32位哦
            for p in range(n - 1):
                y = c_uint32(v[p + 1])
                v[p] = c_uint32(v[p] + mx(z, y, sum1, k, p, e).value).value
                z.value = v[p]
            y = c_uint32(v[0])
            v[n - 1] = c_uint32(v[n - 1] + mx(z, y, sum1,
                                k, n - 1, e).value).value
            z.value = v[n - 1]
            rounds -= 1
        return v

    @staticmethod
    def decrypt(v, n, k=None, delta=0x933779b9):
        if k is None:
            k = [0x45, 0x41, 0x50, 0x494C]
        sum1 = c_uint32(0)
        rounds = 6 + 52 // n
        sum1.value = rounds * delta
        y = c_uint32(v[0])
        e = c_uint32(0)

        while rounds > 0:
            e.value = ((sum1.value >> 2) & 3)
            for p in range(n - 1, 0, -1):
                z = c_uint32(v[p - 1])
                v[p] = c_uint32(v[p] - mx(z, y, sum1, k, p, e).value).value
                y.value = v[p]
            z = c_uint32(v[n - 1])
            v[0] = c_uint32(v[0] - mx(z, y, sum1, k, 0, e).value).value
            y.value = v[0]
            sum1.value -= delta
            rounds -= 1
        return v


class AesKey:
    @staticmethod
    def encrypt(key, iv):
        tea = XXTEA.encrypt(key + iv, 32)
        a = bytearray()
        for i in range(0, 32):
            a.extend(tea[i].to_bytes(4, byteorder='big', signed=False))
        return base64.encodebytes(a).replace(b"\n", b"")

    @staticmethod
    def decrypt(base):
        tea = list()
        byte = base64.decodebytes(base)
        for i in range(0, 32):
            tea.append(int.from_bytes(
                byte[i * 4:i * 4 + 4], byteorder='big', signed=False))
        v = XXTEA.decrypt(tea, 32)
        return v[0:16], v[16:]


# rom file format
# rom size是加密部分长度，不包含文件前36字节
# 其中第一个md5是文件整体加密后的md5
# rom header size是rom json header的大小，需要控制在64KB以下
# rom json header的内容见后面描述
# rom json header之后的内容是全部打包的明文固件数据
#  ________________________________________________
# |4Byte              rom size                     |
# |32Byte             md5 of all(encrypted) below  |
# |4Byte              rom header size(<64KB)       |
# |header sizeByte    rom json header              |
# |xByte              img0                         |
# |xByte              img1                         |
# |xByte              ....                         |
# |________________________________________________|

# rom json header
# script and pre/post are optional
# {
#     "type": "ZN_HY_M4C7S3W0P0",
#     "version": "2.0.20220826.2",
#     "script": {
#         "preSize": 0,
#         "postSize": 0
#     },
#     "img": [
#         {"name": "boot", "size": 0},
#         {"name": "kernel", "size": 0},
#         {"name": "app", "size": 0},
#         {"name": "media", "size": 0}
#     ]
# }
def rom(aes_key, aes_iv, dev_info, out, imgs_dir, partition, img_list, pre_script_file=None, post_script_file=None):
    img_info = pack_prepare(imgs_dir, partition)
    write_file_list = []
    rom_info = RomInfoJson(
        dev_info["inner"]["type"], dev_info["inner"]["version"])

    # 如果存在前处理脚本，则添加到rom json header中
    if pre_script_file is not None and len(pre_script_file):
        rom_info.add_pre_script(os.path.getsize(pre_script_file))
        write_file_list.append(pre_script_file)

    # 如果存在后处理脚本，则添加到rom json header中
    if post_script_file is not None and len(post_script_file):
        rom_info.add_post_script(os.path.getsize(post_script_file))
        write_file_list.append(post_script_file)

    # 将img添加到rom json header中
    for name in img_list:
        rom_info.add_image(os.path.getsize(img_info[name]["file"]), name)
        write_file_list.append(img_info[name]["file"])

    rom_info_json_string = json.dumps(rom_info.final())
    rom_info_json_size = len(rom_info_json_string)
    if rom_info_json_size > 64 * 1024:
        print_err("rom info size {} > 64KB\n".format(rom_info_json_size))
        exit(-1)

    rom_data = bytearray()
    rom_data.extend(rom_info_json_size.to_bytes(
        4, byteorder='big', signed=False))
    rom_data.extend(rom_info_json_string.encode(encoding="utf-8"))
    for file in write_file_list:
        with open(file, "rb") as f:
            rom_data.extend(f.read())
            f.close()
    aes = Aes(aes_key, aes_iv)
    rom_encrypted = aes.encode(rom_data)
    with open(out, 'wb+') as out_file:
        out_file.write(len(rom_encrypted).to_bytes(
            4, byteorder='big', signed=False))
        out_file.write(bytes(Md5.content(rom_encrypted), encoding='utf-8'))
        out_file.write(rom_encrypted)
        out_file.close()


class RomInfoJson:
    def __init__(self, inner_type, inner_version):
        self.info = json.loads("{}")
        self.info["type"] = inner_type
        self.info["version"] = inner_version
        self.img = json.loads("[]")
        self.script = json.loads("{}")

    def add_pre_script(self, size):
        self.script["preSize"] = size

    def add_post_script(self, size):
        self.script["postSize"] = size

    def add_image(self, size, name):
        img = json.loads("{}")
        img["size"] = size
        img["name"] = name
        self.img.append(img)

    def final(self):
        self.info["img"] = self.img
        if "preSize" in self.script or "postSize" in self.script:
            self.info["script"] = self.script
        return self.info


# transform string to int such as (64K)->(65536)
def img_size_str2int(string):
    size = 0
    if string[-1] == 'K':
        size = int(string[0:-1]) * 1024
    elif string[-1] == 'M':
        size = int(string[0:-1]) * 1024 * 1024
    elif string[-1] == 'G':
        size = int(string[0:-1]) * 1024 * 1024 * 1024
    elif string == '-':
        size = 0
    else:
        size = int(string)
    return size


# img_info
# {
#     "boot": {
#         "partSize": 393216,
#         "file": "/the/path/to/fip.bin"
#     },
#     "env": {
#         "partSize": 131072
#     },
#     "kernel": {
#         "partSize": 2490368,
#         "file": "/the/path/to/kernel.bin"
#     },
#     "rootfs": {
#         "partSize": 1048576,
#         "file": "/the/path/to/rootfs.bin"
#     },
#     "media": {
#         "partSize": 5963776,
#         "file": "/the/path/to/media.bin"
#     },
#     "app": {
#         "partSize": 3145728,
#         "file": "/the/path/to/app.bin"
#     },
#     "data": {
#         "partSize": 458752
#     },
# }
def pack_prepare(imgs_dir, partition):
    # 根据partition构建img_info，用于后续打包
    img_info = dict()
    partition_table = partition["partitionTable"]
    for part in partition_table:
        name = part["name"]
        # 过滤掉双系统分区中的另一个
        if '.' in name:
            name, __ = str(name).split('.')
            if name in img_info:
                continue
        one_img = dict()
        one_img["partSize"] = img_size_str2int(part["size"])
        if "file" in part:
            one_img["file"] = imgs_dir + "/" + part["file"]
            if os.path.getsize(one_img["file"]) > one_img["partSize"]:
                print_err("{} img size {} > part size {}".format(name, os.path.getsize(one_img["file"]),
                                                                 one_img["partSize"]))
                exit(-1)
        img_info[name] = one_img
    return img_info


def flash(imgs_dir, partition, out):
    img_info = pack_prepare(imgs_dir, partition)
    # 拷贝分区信息到临时目录
    flash_img_size = img_size_str2int(partition["flashImgSize"])
    writen_size = 0
    with open(out, "wb+") as flash_data:
        for part in partition["partitionTable"]:
            name = part["name"]
            # 双系统分区
            if '.' in name:
                name, __ = str(name).split('.')
            one_img = img_info[name]
            part_size = one_img["partSize"]
            if "file" not in one_img:
                flash_data.write(bytearray([255] * part_size))
            else:
                with open(one_img["file"], "rb") as file:
                    flash_data.write(file.read())
                    flash_data.write(
                        bytearray([255] * (part_size - os.path.getsize(one_img["file"]))))
            writen_size += part_size
        if writen_size > flash_img_size:
            print_err("flash_img_size:[{}] < writen_size:[{}]".format(
                flash_img_size, writen_size))
            exit(-1)
        flash_data.write(bytearray([255] * (flash_img_size - writen_size)))
        flash_data.close()


def parse_rom_info(file, aes_key, aes_iv):
    with open(file, "rb") as f:
        total_size = int.from_bytes(f.read(4), byteorder='big', signed=False)
        total_md5 = f.read(32)
        aes = Aes(aes_key, aes_iv)
        rom_data = aes.decode(f.read())
        rom_info_json_size = int.from_bytes(
            rom_data[0:4], byteorder='big', signed=False)
        print_info("rom info json size = {}".format(rom_info_json_size))
        rom_info_json = json.loads(rom_data[4:rom_info_json_size + 4])
        print_info("rom_info_json = {}".format(
            json.dumps(rom_info_json, indent=2, ensure_ascii=False)))
