#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return [p[j*2:(j+1)*2] for j in range(len(p)//2)]

def processoutput(output, blocksize):
    i=int(b''.join([x for x in output.split(b'\n') if x.find(b'OUTPUT')==0][0][10:].split(b' ')), 16)
    return i

engine=deadpool_dfa.Acquisition(targetbin='./wb_challenge', targetdata='./wb_challenge', goldendata='./wb_challenge.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, addresses=[0x21aa0,0xa0ac0], minfaultspercol=200)
tracefiles=engine.run()
for tracefile in tracefiles:
    if phoenixAES.crack(tracefile):
        break
