#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processinput(iblock, blocksize):
    return (None, ["-key ../target/key.txt -in <(echo %0*x|xxd -r -p) -out >(xxd -p)" % (2*blocksize, iblock)])

def processoutput(output, blocksize):
    return int(''.join([x for x in output.split('\n') if len(x)==32][0]), 16)

filters=[Filter('mem_data_rw1', ['R', 'W'], lambda stack_range, addr, size, data: size == 1, lambda addr, size, data: data & 0xFF, '<B')]

T=TracerGrind('../target/encryptECB', processinput, processoutput, ARCH.amd64, 16, addr_range='0x474b40-0x475df6', filters=filters, shell=True)
#T=TracerPIN('../target/encryptECB', processinput, processoutput, ARCH.amd64, 16, addr_range='0x474b40-0x475df6', filters=filters, shell=True)
T.run(200)
bin2daredevil(keywords=filters,
              configs={'attack_sbox':   {'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX',    'correct_key':'0x693bb79cd7742262c969595c4f8d895f'},
                       'attack_multinv':{'algorithm':'AES', 'position':'LUT/AES_AFTER_MULTINV', 'correct_key':'0x693bb79cd7742262c969595c4f8d895f'}})
