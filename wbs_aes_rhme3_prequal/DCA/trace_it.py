#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processinput(iblock, blocksize):
    return (struct.pack(">QQ", iblock//(2**64), iblock%(2**64)), ["--stdin"])

def processoutput(output, blocksize):
    return int(b''.join(output.strip().split(b' ')), 16)

T=TracerPIN('../target/whitebox', processinput, processoutput, ARCH.amd64, 16, addr_range="0x462886-0x463D6C")
T.run(32)
bin2daredevil()
