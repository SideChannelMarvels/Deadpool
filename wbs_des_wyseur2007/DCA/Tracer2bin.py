#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return [p[j*2:(j+1)*2] for j in range(len(p)/2)]

def processoutput(output, blocksize):
    return int(''.join([x for x in output.split('\n') if x.find('OUTPUT')==0][0][10:].split(' ')), 16)

#T=TracerPIN('../target/wbDES', processinput, processoutput, ARCH.i386, 8)
T=TracerGrind('../target/wbDES', processinput, processoutput, ARCH.i386, 8, addr_range='0x08000000-0x08100000')
T.run(100)
