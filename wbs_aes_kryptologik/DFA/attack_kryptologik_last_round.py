#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return (None, [p[j*2:(j+1)*2] for j in range(len(p)//2)])

def processoutput(output, blocksize):
    return int(output, 16)

engine=deadpool_dfa.Acquisition(targetbin='./DemoKey_table_encrypt', targetdata='DemoKey_table.bin', goldendata='DemoKey_table.bin.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, verbose=2, maxleaf=0x800, minleaf=0x100, outputbeforelastrounds=True)

tracefiles_sets=engine.run()
for tracefile in tracefiles_sets[0]:
    if phoenixAES.crack(tracefile):
        break
