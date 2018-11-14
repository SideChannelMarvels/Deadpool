#!/usr/bin/env python3

import deadpool_dfa
import phoenixAES
import struct

def processinput(iblock, blocksize):
    return (struct.pack(">QQ", iblock//(2**64), iblock%(2**64)), ["--stdin"])

def processoutput(output, blocksize):
    return int(b''.join(output.strip().split(b' ')), 16)

# lazy settings:
#engine=deadpool_dfa.Acquisition(targetbin='./whitebox', targetdata='./whitebox', goldendata='./whitebox.gold',
#        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, verbose=2, minleaf=1, minfaultspercol=100)
# faster settings:
#engine=deadpool_dfa.Acquisition(targetbin='./whitebox', targetdata='./whitebox', goldendata='./whitebox.gold',
#        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, verbose=2,
#        faults=[('nop', lambda x: 0x90)], maxleaf=32, minleaf=1, minleafnail=1, minfaultspercol=2, addresses=(0x45000,0x55000), debug=False)
# damn faster settings:
engine=deadpool_dfa.Acquisition(targetbin='./whitebox', targetdata='./whitebox', goldendata='./whitebox.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, verbose=2,
        maxleaf=1, minleaf=1, minfaultspercol=2, addresses=(0x47bf0,0x47c00), debug=False)
tracefiles_sets=engine.run()
for tracefile in tracefiles_sets[0]:
    if phoenixAES.crack_file(tracefile):
        break
