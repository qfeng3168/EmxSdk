#!/usr/bin/python3
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
