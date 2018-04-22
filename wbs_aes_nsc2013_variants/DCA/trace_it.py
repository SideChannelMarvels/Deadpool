#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processoutput(output, blocksize):
    return int(''.join([x for x in output.split('\n') if x.find('Output')==0][0][10:57].split(' ')), 16)

T=TracerGrind('../target/nosuchcon_2013_whitebox_noenc', processinput, processoutput, ARCH.amd64, 16, filters=[DefaultFilters.stack_w1])
T.run(25)
bin2daredevil(keyword=DefaultFilters.stack_w1, config={'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX', 'correct_key':'0x4b45595f4b45595f4b45595f4b45595f'})
