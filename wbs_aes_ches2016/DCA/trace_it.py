#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return (None, [p[j*2:(j+1)*2] for j in range(len(p)/2)])

def processoutput(output, blocksize):
    return int(''.join([x for x in output.split('\n') if x.find('OUTPUT')==0][0][10:].split(' ')), 16)

T=TracerGrind('../target/wb_challenge', processinput, processoutput, ARCH.amd64, 16,  addr_range='0x108000-0x130000')
# Tracing only the first round:
#T=TracerGrind('../target/wb_challenge', processinput, processoutput, ARCH.amd64, 16,  addr_range='0x108000-0x10c000')
T.run(2000)
bin2daredevil(configs={'attack_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'},
                       'attack_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV'}})
