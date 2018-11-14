#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return (None, [p[j*2:(j+1)*2] for j in range(len(p)//2)])

def processoutput(output, blocksize):
    return int([o[8:] for o in output.split(b'\n') if b"OUTPUT" in o][0].replace(b" ", b""), 16)

#engine=deadpool_dfa.Acquisition(targetbin='./WB_LEE_CASE1_Client', targetdata='Tab.bin', goldendata='Tab.bin.gold', minfaultspercol=200,
engine=deadpool_dfa.Acquisition(targetbin='./WB_LEE_CASE1_Client', targetdata='Tab.bin', goldendata='Tab.bin.gold', addresses=(0xC0000, 0xC3000),
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, verbose=2)
tracefiles_sets=engine.run()
for tracefile in tracefiles_sets[0]:
    if phoenixAES.crack_file(tracefile):
        break
