#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processoutput(output, blocksize):
    return int(output[output.find(b'Output:')+10:].rstrip().replace(b' ', b''), 16)

engine=deadpool_dfa.Acquisition(targetbin='./nosuchcon_2013_whitebox_allenc', targetdata='wbt_allenc', goldendata='wbt_allenc.gold',
        dfa=phoenixAES, processoutput=processoutput, verbose=2)
tracefiles=engine.run()
for tracefile in tracefiles:
    if phoenixAES.crack(tracefile):
        break
