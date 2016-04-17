#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    return ["--load-tables tables_karroumi_noextenc.tbl --input-files <(echo %0*x|xxd -r -p) --out-file >(xxd -p)" % (2*blocksize, iblock)]

def processoutput(output, blocksize):
    return int(''.join([x for x in output.split('\n') if len(x)==32][0]), 16)

engine=deadpool_dfa.Acquisition(targetbin='./main', targetdata='tables_karroumi_noextenc.tbl', goldendata='tables_karroumi_noextenc.tbl.gold', maxleaf=64, minleaf=4, minleafnail=1, addresses=(0x57000, 0x5A000),
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, shell=True, verbose=2)
tracefiles=engine.run()
for tracefile in tracefiles:
    if phoenixAES.crack(tracefile):
        break
