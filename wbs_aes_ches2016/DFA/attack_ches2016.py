#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return (None, [p[j*2:(j+1)*2] for j in range(len(p)//2)])

def processoutput(output, blocksize):
    i=int(b''.join([x for x in output.split(b'\n') if x.find(b'OUTPUT')==0][0][10:].split(b' ')), 16)
    return i

# Initial attack attempt:
#engine=deadpool_dfa.Acquisition(targetbin='./wb_challenge', targetdata='./wb_challenge', goldendata='./wb_challenge.gold',
#        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, addresses=[0x21aa0,0xa0ac0], minfaultspercol=200)
# Because of many false positives, we had to extend minfaultspercol to capture more traces
# But looking at the resulting *.log (cut -c 10-30 *.log |sort|uniq) we see some distinct address ranges producing faults with the right pattern.
# So we can target one of those address ranges and reduce the number of desired traces to the default for a much faster and determinitic attack:

engine=deadpool_dfa.Acquisition(targetbin='./wb_challenge', targetdata='./wb_challenge', goldendata='./wb_challenge.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, addresses=[0x3b000,0x3f000])
tracefiles_sets=engine.run()
for tracefile in tracefiles_sets[0]:
    if phoenixAES.crack(tracefile):
        break
