#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
import struct
from deadpool_dca import *

def processinput(iblock, blocksize):
    assert blocksize==16
    return (struct.pack(">QQ", iblock//(2**64), iblock%(2**64)), [])

def processoutput(output, blocksize):
    assert blocksize==16
    a1, a2=struct.unpack(">QQ", output)
    return a1*(2**64)+a2

T=TracerGrind('../target/main64', processinput, processoutput, ARCH.i386, 16, addr_range="0x407f000-0x040aa000", stack_range="0xffefffd48-0xffefffe90")
T.run(2000)
bin2daredevil(configs={'attack_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'},
                       'attack_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV'}})
