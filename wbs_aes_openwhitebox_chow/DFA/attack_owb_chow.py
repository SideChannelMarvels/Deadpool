#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    return ["-key key.txt -in <(echo %0*x|xxd -r -p) -out >(xxd -p)" % (2*blocksize, iblock)]

def processoutput(output, blocksize):
    return int(''.join([x for x in output.decode().split('\n') if len(x)==32][0]), 16)

engine=deadpool_dfa.Acquisition(targetbin='./encryptECB', targetdata='key.txt', goldendata='key.txt.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, shell=True, minleaf=1, minleafnail=1, minfaultspercol=100, verbose=2)
#        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, shell=True, minleaf=1, minleafnail=1, minfaultspercol=150, verbose=2)
tracefiles=engine.run()
for tracefile in tracefiles:
    if phoenixAES.crack(tracefile):
        break
