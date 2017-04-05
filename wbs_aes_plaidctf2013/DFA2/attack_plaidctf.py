#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    # hardcoded in spawn_drmless.py
    return (None, None)

def processoutput(output, blocksize):
# Typical output (Frida mem dump):
#- offset -   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  0123456789ABCDEF
#0x00000000  34 1a fa 02 f2 95 e6 fc 0b 08 9e 41 c6 30 27 4b  4..........A.0'K
    return int(output[output.find(b'0x00000000')+12:output.find(b'0x00000000')+59].replace(b' ', b''), 16)

# Key found in  1m50s when addresses are restricted to the sweet spot:
engine=deadpool_dfa.Acquisition(targetbin='./spawn_drmless.py', targetdata='./drmless', goldendata='drmless.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, maxleaf=2048, faults=[('nop', lambda x: 0x90)], addresses=(0x1c0000, 0x1d0000), verbose=2)
# Key found in 12m40s when addresses are not specified:
#engine=deadpool_dfa.Acquisition(targetbin='./spawn_drmless.py', targetdata='./drmless', goldendata='drmless.gold',
#        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, maxleaf=2048, faults=[('nop', lambda x: 0x90)], verbose=2)
tracefiles=engine.run()
for trace in tracefiles:
    if phoenixAES.crack(trace, encrypt=False):
        break
