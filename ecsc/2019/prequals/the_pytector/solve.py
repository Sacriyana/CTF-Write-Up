# uncompyle6 version 3.3.2
# Python bytecode 3.7 (3394)
# Decompiled from: Python 3.7.3 (default, May 16 2019, 12:35:00) 
# [GCC 8.3.0]
# Embedded file name: fake_check.py
# Size of source mod 2**32: 298 bytes
#coding: utf-8


# 420d 0d0a 0000 0000 ace7 da5c 2a01 0000

import string

def parse_array(array):
    res = []
    for a in array:
        if a.startswith('0x'):
            res.append(int(a,16))
        else:
            res.append(int(a,10))
    return res

key = b'Complex is better than complicated'

# Sans le changement : noter la disparation de parenthése
def check(serial):
    global p
    global passwd
    serialsize = len(serial)
    key = b'Complex is better than complicated'
    result = []
    for i in range(serialsize):
        result.append(((serial[p[i]] ^ key[(i % len(key))]) << 2) - 4)

    return result == passwd


def reverse_serial_1():
    passwd = [432, 360, 264, 184, 56, 344, 340, 268, 280, 172, 64, 300, 320, 356, 356, 296, 328, 504, 356, 276, -4, 348, 52, 304, 256, 264, 184, 48, 280, 344, 312, 168, 296, -4, 128, 256, 264, 184, 40, 296, 340, 52, 280, 80, 504, 300, 296]
    p = [43, 8, 26, 11, 3, 1, 46, 10, 28, 23, 13, 33, 0, 45, 34, 37, 16, 5, 36, 32, 9, 4, 20, 19, 25, 22, 17, 7, 27, 14, 42, 41, 18, 2, 6, 40, 30, 29, 15, 21, 44, 24, 39, 12, 35, 38, 31]
    serial = [' '] * 47
    for i in range(0,47):
        for c in string.printable:
            result = ((ord(c) ^ key[i % len(key)]) << 2) - 4
            if result == passwd[i]:
                serial[p[i]] = c
                break
    return ''.join(serial)
# 42dc6_ba4ad_f14g!_....._....._....._....._.....

# Avec le changement dans check
def check(serial):
    global p
    global passwd
    serialsize = len(serial)
    key = b'Complex is better than complicated'
    result = []
    for i in range(serialsize):
        result.append((serial[p[i]] ^ key[(i % len(key))] << 4) + 42)

    return result == passwd

def reverse_serial_2():
    p = ["0x0c","0x24","0x17","0x11","0x1b","0x22","0x12","0x19","0x21","0x2a","0x16","0x15","0x2d","0x14","0x23","0x0d","0x1e","0x26","0x1f","0x1c","0x1a","0x0a","0x2c","0x1d","0x09","0x0b","0x02","0x04","0x0e","0x01","0x25","0x0f","0x29","0x13","0x27","0x18","0x06","0x07","0x2e","0x20","0x05","0x08","0x00","0x03","0x10","0x2b","0x28"]
    p = parse_array(p)
    passwd = ["0x47b","0x6f2","0x6b9","0x789","0x71d","0x690","0x810","0x28e","0x71e","0x732","0x28b","0x642","0x65b","0x7a0","0x749","0x65d","0x76b","0x260","0x79f","0x6db","0x64a","0x6af","0x263","0x699","0x6ee","0x6b9","0x78b","0x71f","0x6cb","0x62e","0x653","0x7a2","0x639","0x69d","0x47d","0x6ec","0x70a","0x78c","0x6cb","0x690","0x809","0x25c","0x720","0x72a","0x260","0x63e","0x65c"]
    passwd = parse_array(passwd)
    serial = [' '] * 47
    for i in range(0,47):
        for c in string.printable:
            result = (ord(c) ^ key[(i % len(key))] << 4) + 42
            if result == passwd[i]:
                serial[p[i]] = c
                break
    return ''.join(serial)
# f4a05_0b24e_ac186_f368a_2d031_a56d6_896cb_849aa

def main():
    print(reverse_serial_1())
    print(reverse_serial_2())

if __name__ == '__main__':
    main()
# okay decompiling fake_check.pyc