#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processinput(iblock, blocksize):
    return ["--load-tables tables_karroumi_noextenc.tbl --input-files <(echo %0*x|xxd -r -p) --out-file >(xxd -p)" % (2*blocksize, iblock)]

def processoutput(output, blocksize):
    return int(''.join([x for x in output.split('\n') if len(x)==32][0]), 16)

filters=[Filter('mem_addr1_rw1', ['R', 'W'], lambda stack_range, addr, size, data: addr < 0x7ffffff68000 and size == 1, lambda addr, size, data: addr & 0xFF, '<B')]
#filters=[Filter('mem_addr1_rw1', ['R', 'W'], lambda stack_range, addr, size, data: addr < 0x7ffffff68000 and size == 1, lambda addr, size, data: addr & 0xFF, '<B'),
#         Filter('mem_data_rw1',  ['R', 'W'], lambda stack_range, addr, size, data: addr < 0x7ffffff68000 and size == 1, lambda addr, size, data: data, '<B')]

T=TracerPIN('../target/main', processinput, processoutput, ARCH.amd64, 16, addr_range='0x4512c0-0x452477', filters=filters, shell=True)
T.run(2000)
bin2daredevil(keywords=filters,
              configs={'attack_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX',    'correct_key':'0x2b7e151628aed2a6abf7158809cf4f3c'},
                       'attack_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV', 'correct_key':'0x2b7e151628aed2a6abf7158809cf4f3c'}})
