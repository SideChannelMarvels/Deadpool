#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return (None, [p[j*2:(j+1)*2] for j in range(len(p)//2)])

def processoutput(output, blocksize):
    return int([o[8:] for o in output.split('\n') if "OUTPUT" in o][0].replace(" ", ""), 16)

filters=[Filter('mem_addr1_r1', ['R'], lambda stack_range, addr, size, data: addr > 0x7ffffff36c00 and size == 1, lambda addr, size, data: addr & 0xFF, '<B'),
         Filter('mem_data_r1',  ['R'], lambda stack_range, addr, size, data: addr > 0x7ffffff36c00 and size == 1, lambda addr, size, data: data, '<B'),
         Filter('mem_stack_w1',  ['W'], lambda stack_range, addr, size, data: addr < 0x7ffffff36c00 and size == 1, lambda addr, size, data: data, '<B')]

T=TracerPIN('../target/WB_LEE_CASE1_Client', processinput, processoutput, ARCH.amd64, 16, filters=filters)
T.run(2000)
bin2daredevil(keywords=filters, config={'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'})
