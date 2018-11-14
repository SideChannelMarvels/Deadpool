#!/usr/bin/env python3

import deadpool_dfa
import phoenixAES
import struct

def processinput(iblock, blocksize):
    return (bytes.fromhex('%0*x' % (2*blocksize, iblock)), None)

def processoutput(output, blocksize):
    return int.from_bytes(output, byteorder='big', signed=False)

#engine=deadpool_dfa.Acquisition(targetbin='./main64', targetdata='./libnative-lib.so', goldendata='./libnative-lib.so.gold',
#        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, verbose=2, minleaf=1, minleafnail=1)

# Limited address range to .rodata section for faster results:
engine=deadpool_dfa.Acquisition(targetbin='./main64', targetdata='./libnative-lib.so', goldendata='./libnative-lib.so.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, verbose=2, minleaf=1, minleafnail=1, addresses=[0x6350,0x2b490])

tracefiles_sets=engine.run()
for tracefile in tracefiles_sets[0]:
    if phoenixAES.crack_file(tracefile):
        break
