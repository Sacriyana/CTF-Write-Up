# uncompyle6 version 3.3.2
# Python bytecode 3.7 (3394)
# Decompiled from: Python 3.7.3 (default, May 16 2019, 12:35:00) 
# [GCC 8.3.0]
# Embedded file name: fake_check.py
# Size of source mod 2**32: 298 bytes
passwd = [
 432, 360, 264, 184, 56, 344, 340, 268, 280, 172, 64, 300, 320, 356, 356, 296, 328, 504, 356, 276, -4, 348, 52, 304, 256, 264, 184, 48, 280, 344, 312, 168, 296, -4, 128, 256, 264, 184, 40, 296, 340, 52, 280, 80, 504, 300, 296]
p = [43, 8, 26, 11, 3, 1, 46, 10, 28, 23, 13, 33, 0, 45, 34, 37, 16, 5, 36, 32, 9, 4, 20, 19, 25, 22, 17, 7, 27, 14, 42, 41, 18, 2, 6, 40, 30, 29, 15, 21, 44, 24, 39, 12, 35, 38, 31]

def check(serial):
    global p
    global passwd
    serialsize = len(serial)
    key = b'Complex is better than complicated'
    result = []
    for i in range(serialsize):
        result.append(((serial[p[i]] ^ key[(i % len(key))]) << 2) - 4)

    return result == passwd


def successful(serial):
    print('Good job!\nECSC{{{}}}'.format(serial.decode('utf-8')))


def defeated():
    print('Not really')


def main():
    import pytector
    serial = input('Serial number: ').encode('utf-8')
    if check(serial):
        successful(serial)
    else:
        defeated()


if __name__ == '__main__':
    main()
# okay decompiling fake_check.pyc