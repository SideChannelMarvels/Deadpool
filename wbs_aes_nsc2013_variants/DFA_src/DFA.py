#!/usr/bin/env python3

import subprocess
import struct
import sys
sys.path.insert(0, '../../../JeanGrey/phoenixAES/')
import phoenixAES

TRACES=20
CHALLENGE='./nosuchcon_2013_whitebox_allenc'
BLOCKSIZE=16
TRACEFILE='dfa.txt'

pairs=[]
plain=0x000102030405060708090a0b0c0d0e0f
with open(TRACEFILE, 'wb') as tracefile:
    for i in range(TRACES):
        output=subprocess.check_output([CHALLENGE, '%0*x' % (2*BLOCKSIZE, plain)])
        cipher=int(output[output.find(b'Output:')+10:].rstrip().replace(b' ', b''), 16)
        print ('%05i %0*X -> %0*X' % (i, 2*BLOCKSIZE, plain, 2*BLOCKSIZE, cipher))
        tracefile.write(('%0*X %0*X\n' % (2*BLOCKSIZE, plain, 2*BLOCKSIZE, cipher)).encode('utf8'))
        if i==0:
            CHALLENGE=CHALLENGE+"_DFA"
phoenixAES.crack_file(TRACEFILE)
