#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-

import zlib
import struct
import sys
import random
import re
import json

"""
before schedule

    0x555556257750 <crc>:   0xf9f01fe54457049b      0xb852e068da544ae8
    0x555556257760 <shiftedmask>:   0x88cd4eed2d1c9cb7      0x28d994ee53d9d327
    0x555556257770 <mask3>: 0x271d7b03298f724e      0xd81c681895e3fd1e
    0x555556257780 <mask>:  0xbf5f2faeb5fe6b8c      0xa61c554d77db5fff

    sboxes 0x203620
    mask & mask2

    scheduling 0x114c:
      every byte is obfuscated for 40 cases, 0 - 39

    figure out what is this encryption scheme

    Rijndael key schedule, part of aes
"""

BASE = 0x555555554000
schedule = 0x114c
states = 0xD03660
mask_0 = 0xD03780
mask_3 = 0xD03770
shiftedmask = 0xD03760

brs = {
    "before shiftrow": 0xe34,
    "after mix_col": 0x1023,
    "gen mask": 0xe28,
    "mask3 final": 0x1030,
    "switch": 0x173d,
    "sboxes": 0x203620,
    "xor state": 0x205e,
    "rounds": 0xd037a0,
    "br1": 0x11C9,
    "br2": 0x1C1A,
}

class Schedule(object):
    def __init__(self):
        with open('sboxes', 'rb') as f:
            sboxes = list(f.read())
        self.sboxes = map(ord, sboxes)
        self.inv_sboxes = self.gen_invsboxes()
        self.crc = [0x9b,0x04,0x57,0x44,0xe5,0x1f,0xf0,0xf9,
                    0xe8,0x4a,0x54,0xda,0x68,0xe0,0x52,0xb8]        # file crc
        self._init_masks()

    def encrypt(self, data):
        assert len(data) == 32
        self.input_ = map(ord, list(data.decode('hex')))
        self._scramble()
        # debug
        # self.states[0] = [0, 1, 0, 0, 0, 0, 0, 0,
        #                   0, 0, 0, 0, 0, 0, 0, 0]
        self.schedule()

    def decrypt(self):
        pass

    def _scramble(self):        # the step 1 before schedueling
        self.m = [[0x6, 0xc, 0x3, 0x5, 0x8, 0xe, 0x7, 0x0, 0xd, 0x2, 0x9, 0x1, 0xf, 0xb, 0x4, 0xa],
                  [0xf, 0xc, 0x6, 0xa, 0x9, 0x5, 0x2, 0x0, 0x7, 0x1, 0x8, 0xb, 0xd, 0x3, 0xe, 0x4],
                  [0xa, 0x3, 0xd, 0xc, 0x6, 0xe, 0x1, 0x4, 0xb, 0x5, 0x7, 0xf, 0x2, 0x0, 0x8, 0x9],
                  [0xa, 0x5, 0x3, 0x1, 0xb, 0xd, 0x6, 0xc, 0x2, 0xf, 0x4, 0x8, 0xe, 0x7, 0x9, 0x0],
                  [0x0, 0x6, 0xb, 0x7, 0xf, 0x5, 0x8, 0x3, 0x1, 0x4, 0x9, 0x2, 0xc, 0xd, 0xa, 0xe],
                  [0x2, 0x0, 0x3, 0xb, 0xc, 0x8, 0x4, 0xd, 0x7, 0x5, 0xa, 0x1, 0x6, 0xf, 0xe, 0x9],
                  [0xc, 0xf, 0x0, 0x4, 0xb, 0x6, 0xa, 0x1, 0x9, 0x5, 0xe, 0x3, 0x2, 0x7, 0x8, 0xd],
                  [0x0, 0x4, 0x8, 0xc, 0x1, 0x5, 0x9, 0xd, 0x2, 0x6, 0xa, 0xe, 0x3, 0x7, 0xb, 0xf],
                  [0x6, 0xc, 0x3, 0x5, 0x8, 0xe, 0x7, 0x0, 0xd, 0x2, 0x9, 0x1, 0xf, 0xb, 0x4, 0xa],
                  [0xf, 0xc, 0x6, 0xa, 0x9, 0x5, 0x2, 0x0, 0x7, 0x1, 0x8, 0xb, 0xd, 0x3, 0xe, 0x4],
                  [0xa, 0x3, 0xd, 0xc, 0x6, 0xe, 0x1, 0x4, 0xb, 0x5, 0x7, 0xf, 0x2, 0x0, 0x8, 0x9],
                  [0xa, 0x5, 0x3, 0x1, 0xb, 0xd, 0x6, 0xc, 0x2, 0xf, 0x4, 0x8, 0xe, 0x7, 0x9, 0x0],
                  [0x0, 0x6, 0xb, 0x7, 0xf, 0x5, 0x8, 0x3, 0x1, 0x4, 0x9, 0x2, 0xc, 0xd, 0xa, 0xe],
                  [0x2, 0x0, 0x3, 0xb, 0xc, 0x8, 0x4, 0xd, 0x7, 0x5, 0xa, 0x1, 0x6, 0xf, 0xe, 0x9],
                  [0xc, 0xf, 0x0, 0x4, 0xb, 0x6, 0xa, 0x1, 0x9, 0x5, 0xe, 0x3, 0x2, 0x7, 0x8, 0xd]]

        extended = []
        for r in self.m:
            extended.append(map(lambda x: self.input_[x], r))
        self.states = extended

    def _unscramble(self):
        state7 = self.states[7]
        password = [0] * 16
        for idx in self.m[7][::-1]:
            password[idx] = state7.pop()

        return "".join(map(lambda x: "%02X" % x, password))

    def print_states(self):
        for idx, state in enumerate(self.states):
            print hex(idx),": ",map(hex, state)

    def _init_masks(self):
        rands = [random.randint(0,255) for i in xrange(16)]
        # mask value doesn't matter
        rands = [0x0 for i in xrange(16)]
        self.mask0 = list(rands)
        self.mask3 = list(rands)
        self.shiftedmask = list(rands)

        self.shiftedmask = self.shiftrows(self.shiftedmask)
        self.mask3 = self.shiftrows(self.mask3)
        self.mix_cols(self.mask3)
        self.mask3 = self.xor_arr(self.mask3, self.mask0)

    def mix_cols(self, mem):
        for i in xrange(4):
            col = []
            for j in xrange(4):
                col.append(mem[i + j*4])
            col_n = self._mix_col(col)
            for j in xrange(4):
                mem[i + j*4] = col_n[j]

        return mem

    def _mix_col(self, col):
        """
        https://github.com/renanpolisciuc/aes/blob/e5daf1ca1cfc87150c50e3de594cdac14829079e/cpu_src/tables.cpp
        """
        col_n = list(col)
        mul2 = self.mul2
        mul3 = self.mul3

        col_n[0] = mul3(col[1]) ^ mul2(col[0]) ^ col[2] ^ col[3]
        col_n[1] = mul3(col[2]) ^ mul2(col[1]) ^ col[0] ^ col[3]
        col_n[2] = mul3(col[3]) ^ mul2(col[2]) ^ col[1] ^ col[0]
        col_n[3] = mul3(col[0]) ^ mul2(col[3]) ^ col[2] ^ col[1]

        return col_n

    @staticmethod
    def BYTE(self, v, offset):
        return (v >> offset*8) &0xFF

    def unschedule(self, slot=None, data=None):
        xor = self.xor_arr
        BYTE = self.BYTE
        mask0 = self.mask0
        mask3 = self.mask3
        shiftedmask = self.shiftedmask
        sboxes = self.sboxes
        crc = self.crc
        mul9 = lambda x: self.g_mul(x, 9)
        mul11 = lambda x: self.g_mul(x, 11)
        mul13 = lambda x: self.g_mul(x, 13)
        mul14 = lambda x: self.g_mul(x, 14)

        self.state_rnd = [40] * 15
        rnd = 0

        if slot != None and data != None:
            self.states = [data] * 15
            self.state_rnd = [0] * 15
            self.state_rnd[slot] = 40

        while any(map(lambda x: x^rnd, self.state_rnd)):
            slot = random.randint(0, 14)
            cur_rnd = self.state_rnd[slot]
            cur_state = self.states[slot]
            if cur_rnd == rnd:
                continue

            if cur_rnd in [40]:
                cur_state = xor(cur_state, shiftedmask)

            elif cur_rnd in [39, 35, 31, 27, 23, 19, 15, 11, 7, 3]:
                tmp = list(cur_state)
                tmp[4] = cur_state[7]
                tmp[5] = cur_state[4]
                tmp[6] = cur_state[5]
                tmp[7] = cur_state[6]
                tmp[8] = cur_state[10]
                tmp[9] = cur_state[11]
                tmp[10] = cur_state[8]
                tmp[11] = cur_state[9]
                tmp[12] = cur_state[13]
                tmp[13] = cur_state[14]
                tmp[14] = cur_state[15]
                tmp[15] = cur_state[12]
                cur_state = tmp

            elif cur_rnd in [38, 34, 30, 26, 22, 18, 14, 10, 6, 2]:
                tmp = [0] * 16
                cur_state = xor(cur_state, crc)
                for pos in xrange(16):
                    tmp[pos]= self.inv_sboxes[cur_rnd-1][pos][cur_state[pos]]
                cur_state = tmp

            elif cur_rnd in [37, 33, 29, 25, 21, 17, 13, 9, 5]:
                cur_state = xor(cur_state, mask3)

            elif cur_rnd in [36, 32, 28, 24, 20, 16, 12, 8, 4]:
                """ https://gist.github.com/raullenchai/2920069 """
                R0 = mul11(cur_state[4]) ^ mul14(cur_state[0]) ^ mul9(cur_state[12]) ^ mul13(cur_state[8])
                R1 = mul11(cur_state[5]) ^ mul14(cur_state[1]) ^ mul9(cur_state[13]) ^ mul13(cur_state[9])
                R2 = mul11(cur_state[6]) ^ mul14(cur_state[2]) ^ mul9(cur_state[14]) ^ mul13(cur_state[10])
                R3 = mul11(cur_state[7]) ^ mul14(cur_state[3]) ^ mul9(cur_state[15]) ^ mul13(cur_state[11])

                R4 = mul11(cur_state[8]) ^ mul14(cur_state[4]) ^ mul13(cur_state[12]) ^ mul9(cur_state[0])
                R5 = mul11(cur_state[9]) ^ mul14(cur_state[5]) ^ mul13(cur_state[13]) ^ mul9(cur_state[1])
                R6 = mul11(cur_state[10]) ^ mul14(cur_state[6]) ^ mul13(cur_state[14]) ^ mul9(cur_state[2])
                R7 = mul11(cur_state[11]) ^ mul14(cur_state[7]) ^ mul13(cur_state[15]) ^ mul9(cur_state[3])

                R8 = mul11(cur_state[12]) ^ mul14(cur_state[8]) ^ mul9(cur_state[4]) ^ mul13(cur_state[0])
                R9 = mul11(cur_state[13]) ^ mul14(cur_state[9]) ^ mul9(cur_state[5]) ^ mul13(cur_state[1])
                Ra = mul11(cur_state[14]) ^ mul14(cur_state[10]) ^ mul9(cur_state[6]) ^ mul13(cur_state[2])
                Rb = mul11(cur_state[15]) ^ mul14(cur_state[11]) ^ mul9(cur_state[7]) ^ mul13(cur_state[3])

                Rc = mul11(cur_state[0]) ^ mul14(cur_state[12]) ^ mul9(cur_state[8]) ^ mul13(cur_state[4])
                Rd = mul11(cur_state[1]) ^ mul14(cur_state[13]) ^ mul9(cur_state[9]) ^ mul13(cur_state[5])
                Re = mul11(cur_state[2]) ^ mul14(cur_state[14]) ^ mul9(cur_state[10]) ^ mul13(cur_state[6])
                Rf = mul11(cur_state[3]) ^ mul14(cur_state[15]) ^ mul9(cur_state[11]) ^ mul13(cur_state[7])
                cur_state = [R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,Ra,Rb,Rc,Rd,Re,Rf]

            elif cur_rnd in [1]:
                cur_state = xor(cur_state, mask0)

            self.states[slot] = cur_state
            self.state_rnd[slot] -= 1

    def gen_invsboxes(self):
        """
        inv_sboxes[rnd][pos][in_byte] -> out_byte
        """
        inv_sboxes = {}
        rnds = [1, 5, 9, 13, 17, 21, 25, 29, 33, 37]
        for rnd in rnds:
            v55 = rnd >> 2
            v53 = v55 << 20
            inv_sboxes[rnd] = {}
            for pos in xrange(16):
                inv_sboxes[rnd][pos] = [0] * 256
                for in_byte in xrange(0, 256):
                    if pos == 0:
                        P = 256 * (in_byte + (v55 << 12))
                    else:
                        P = 256 * in_byte + v53
                    out_byte = self.sboxes[P + pos * 0x10000]
                    inv_sboxes[rnd][pos][out_byte] = in_byte

        return inv_sboxes

    def schedule(self):
        xor = self.xor_arr
        BYTE = self.BYTE
        mask0 = self.mask0
        mask3 = self.mask3
        shiftedmask = self.shiftedmask
        sboxes = self.sboxes
        crc = self.crc
        mul2 = self.mul2
        mul3 = self.mul3

        self.state_rnd = [0] * 15
        # ------------------- debug -----------------------
        # self.states[0] = [1, 2, 0, 0, 0, 0, 0, 0,
        #                   0, 0, 0, 0, 0, 0, 0, 0]
        # ------------------- debug -----------------------

        rnd = 40
        while any(map(lambda x: x^rnd, self.state_rnd)):
            slot = random.randint(0, 14)
            # ------------------- debug -----------------------
            # slot = 13
            # ------------------- debug -----------------------

            cur_rnd = self.state_rnd[slot]
            cur_state = self.states[slot]
            if cur_rnd == rnd:
                continue

            if cur_rnd in [0]:
                cur_state = xor(cur_state, mask0)

            elif cur_rnd in [1, 5, 9, 13, 17, 21, 25, 29, 33, 37]:
                v55 = cur_rnd >> 2
                v53 = v55 << 20
                tmp = [0] * 16
                for pos in xrange(16):
                    if pos == 0:
                        P = 256 * (cur_state[pos] + (v55 << 12))
                    else:
                        P = 256 * cur_state[pos] + v53
                    tmp[pos] = self.sboxes[P + pos * 0x10000]

                cur_state = xor(tmp, crc)

            elif cur_rnd in [2, 6, 10, 14, 18, 22, 26, 30, 34, 38]:
                tmp = list(cur_state)
                tmp[4] = cur_state[5]
                tmp[5] = cur_state[6]
                tmp[6] = cur_state[7]
                tmp[7] = cur_state[4]

                tmp[8] = cur_state[10]
                tmp[9] = cur_state[11]
                tmp[10] = cur_state[8]
                tmp[11] = cur_state[9]

                tmp[12] = cur_state[15]
                tmp[13] = cur_state[12]
                tmp[14] = cur_state[13]
                tmp[15] = cur_state[14]
                cur_state= tmp

            elif cur_rnd in [3, 7, 11, 15, 19, 23, 27, 31, 35]:
                R0 = mul3(cur_state[4]) ^ mul2(cur_state[0]) ^ cur_state[12] ^ cur_state[8]
                R1 = mul3(cur_state[5]) ^ mul2(cur_state[1]) ^ cur_state[13] ^ cur_state[9]
                R2 = mul3(cur_state[6]) ^ mul2(cur_state[2]) ^ cur_state[14] ^ cur_state[10]
                R3 = mul3(cur_state[7]) ^ mul2(cur_state[3]) ^ cur_state[15] ^ cur_state[11]

                R4 = mul3(cur_state[8]) ^ mul2(cur_state[4]) ^ cur_state[12] ^ cur_state[0]
                R5 = mul3(cur_state[9]) ^ mul2(cur_state[5]) ^ cur_state[13] ^ cur_state[1]
                R6 = mul3(cur_state[10]) ^ mul2(cur_state[6]) ^ cur_state[14] ^ cur_state[2];
                R7 = mul3(cur_state[11]) ^ mul2(cur_state[7]) ^ cur_state[15] ^ cur_state[3];

                R8 = mul3(cur_state[12]) ^ mul2(cur_state[8]) ^ cur_state[4] ^ cur_state[0]
                R9 = mul3(cur_state[13]) ^ mul2(cur_state[9]) ^ cur_state[5] ^ cur_state[1]
                Ra = mul3(cur_state[14]) ^ mul2(cur_state[10]) ^ cur_state[6] ^ cur_state[2]
                Rb = mul3(cur_state[15]) ^ mul2(cur_state[11]) ^ cur_state[7] ^ cur_state[3]

                Rc = mul3(cur_state[0]) ^ mul2(cur_state[12]) ^ cur_state[8] ^ cur_state[4]
                Rd = mul3(cur_state[1]) ^ mul2(cur_state[13]) ^ cur_state[9] ^ cur_state[5]
                Re = mul3(cur_state[2]) ^ mul2(cur_state[14]) ^ cur_state[10] ^ cur_state[6]
                Rf = mul3(cur_state[3]) ^ mul2(cur_state[15]) ^ cur_state[11] ^ cur_state[7]
                cur_state = [R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,Ra,Rb,Rc,Rd,Re,Rf]

            elif cur_rnd in [4, 8, 12, 16, 20, 24, 28, 32, 36]:
                cur_state = xor(cur_state, mask3)

            elif cur_rnd in [39]:
                cur_state = xor(cur_state, shiftedmask)

            self.states[slot] = cur_state
            self.state_rnd[slot] += 1

    def xor_arr(self, a1, a2):
        return [i ^ j for i, j in zip(a1, a2)]

    def check_sum(self):
        self.chksum = [0] * 16
        for state in self.states:
            self.chksum = self.xor_arr(state, self.chksum)
        print "chech sum: ", map(hex, self.chksum)
        return self.chksum

    @staticmethod
    def shiftrows(mem):
        new_mem = list(mem)
        new_mem[6] = mem[7]
        new_mem[5] = mem[6]
        new_mem[4] = mem[5]
        new_mem[7] = mem[4]
        new_mem[10] = mem[8]
        new_mem[9] = mem[11]
        new_mem[8] = mem[10]
        new_mem[11] = mem[9]
        new_mem[13] = mem[12]
        new_mem[14] = mem[13]
        new_mem[12] = mem[15]
        new_mem[15] = mem[14]
        return new_mem

    @staticmethod
    def g_mul(a, b):
        """ https://gist.github.com/raullenchai/2920069 """
        a = ord(a) if isinstance(a, str) else a
        b = ord(b) if isinstance(b, str) else b
        p = 0
        hiBitSet = 0
        for i in range(8):
            if b & 1 == 1:
                p ^= a
            hiBitSet = a & 0x80
            a <<= 1
            if hiBitSet == 0x80:
                a ^= 0x1b
            b >>= 1
        return p % 256

    def mul2(self, num):
        return self.g_mul(num, 2)

    def mul3(self, num):
        return self.g_mul(num, 3)


def test(serial_num):
    print hex(crc(serial_num))

def crc(data):
    return zlib.crc32(data) & 0xFFFFFFFF

def get_addr(offset):
    return hex(BASE + offset)

def solve():
    """
    final result = [0x13, 0x13, 0x13, 0x13, 0x37, 0x37, 0x37, 0x37, 0x69, 0x69, 0x69, 0x69, 0x42, 0x42, 0x42, 0x42]

    final xor result == slot[7]
    """
    s = Schedule()
    s.encrypt("00112233445566778899aabbccddccff")
    s.check_sum()
    s.unschedule(slot=7, data = [0x13, 0x13, 0x13, 0x13, 0x37, 0x37, 0x37, 0x37, 0x69, 0x69, 0x69, 0x69, 0x42, 0x42, 0x42, 0x42])
    s.print_states()
    init_state = ['0xa', '0xe', '0xc2', '0x74', '0xb7', '0xc6', '0x41', '0x70', '0x98', '0x5f', '0x2d', '0xd7', '0x2c', '0xc9', '0x52', '0x68']
    print "password: ", s._unscramble()

def main():
    print "schedule: ", get_addr(schedule)
    print "states: ", get_addr(states)
    print "mask0: ", get_addr(mask_0)
    print "mask3: ", get_addr(mask_3)
    print "shiftedmask: ", get_addr(shiftedmask)
    for w, offset in brs.iteritems():
        print "{0}: ".format(w), get_addr(offset)
    print "="*40

if __name__ == "__main__":
    if "debug" in sys.argv:
        main()
    else:
        solve()
