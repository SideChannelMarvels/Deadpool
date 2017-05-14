#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    # will require shell=True...
    return (None, ['-f', '-E', '<(echo %s|xxd -r -p)' % (p*4)])

def processoutput(output, blocksize):
    return int(output[:16].encode('hex'), 16)

# Patch drmless to always return decrypted version:
if not os.path.isfile('drmless'):
    with open('../target/drmless', 'rb') as finput, open('drmless', 'wb') as foutput:
        foutput.write(finput.read(0x6C18)+b'\x01'+finput.read()[1:])
    os.chmod('drmless', 0o755)

T=TracerGrind('./drmless', processinput, processoutput, ARCH.amd64, 16, addr_range="0x8054060-0x82a2279", stack_range="0xffffa000-0xffffffff", filters=[DefaultFilters.stack_w1, DefaultFilters.stack_w4], shell=True)
T.run(200)
bin2daredevil(configs={
        'attack_input':{'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOXINV', 'correct_key':'0xFAEF63792F9A97A1FB78C88C4CA7048F'},
        'attack_output':{'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX', 'guess':'output', 'correct_key':'0xB1BA2737C83233FE7F7A7DF0FBB01D4A'},
}, keywords=[DefaultFilters.stack_w1, DefaultFilters.stack_w4])
