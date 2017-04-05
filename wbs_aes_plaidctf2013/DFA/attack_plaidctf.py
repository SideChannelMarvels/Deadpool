#!/usr/bin/env python3

import sys
import os
import deadpool_dfa
import phoenixAES
import binascii

def processinput(iblock, blocksize):
    #p=b'%0*x' % (2*blocksize, iblock)  # Requires python3.5
    p=('%0*x' % (2*blocksize, iblock)).encode('utf8')
    open('foo', 'wb').write(binascii.unhexlify(p)*4)
    return (None, ['-f', '-E', 'foo'])

def processoutput(output, blocksize):
    return int(binascii.hexlify(output[:16]), 16)

# Patch drmless to always return decrypted version:
if not os.path.isfile('drmless.gold'):
    with open('drmless', 'rb') as finput, open('drmless.gold', 'wb') as foutput:
        foutput.write(finput.read(0x6C18)+b'\x01'+finput.read()[1:])

engine=deadpool_dfa.Acquisition(targetbin='./drmless', targetdata='./drmless', goldendata='drmless.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, maxleaf=2048, faults=[('nop', lambda x: 0x90)], verbose=2)
tracefiles_sets=engine.run()
for trace in tracefiles_sets[1]:
    if phoenixAES.crack(trace, encrypt=False):
        break
