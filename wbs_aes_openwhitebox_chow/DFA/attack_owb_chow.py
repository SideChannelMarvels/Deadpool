#!/usr/bin/env python3

import sys
import deadpool_dfa
import phoenixAES

def processinput(iblock, blocksize):
    return (None, ["-key key.txt -in <(echo %0*x|xxd -r -p) -out >(xxd -p)" % (2*blocksize, iblock)])

def processoutput(output, blocksize):
    return int(''.join([x for x in output.decode().split('\n') if len(x)==32][0]), 16)

# Initial attack attempt:
#engine=deadpool_dfa.Acquisition(targetbin='./encryptECB', targetdata='key.txt', goldendata='key.txt.gold',
#        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, shell=True, minleaf=1, minleafnail=1, minfaultspercol=150, verbose=2)
# Because of many false positives, we had to extend minfaultspercol to capture more traces
# But looking at the resulting *.log (cut -c 10-30 *.log |sort|uniq) we see some distinct address ranges producing faults with the right pattern:
# 0x3f200-0x43a00
# 0x5b000-0x5df00
# 0x7a100-0x7d000
# 0x97000-0x9a000

# So we can target one of those address ranges and reduce the number of desired traces to the default for a much faster and determinitic attack:
engine=deadpool_dfa.Acquisition(targetbin='./encryptECB', targetdata='key.txt', goldendata='key.txt.gold',
        dfa=phoenixAES, processinput=processinput, processoutput=processoutput, shell=True, minleaf=1, minleafnail=1, addresses=(0x7a100,0x7d000), verbose=2)
# Tests show that on those 4 address ranges:
# 0x3f200-0x43a00 -> no key found
# 0x5b000-0x5df00 -> no key found
# 0x7a100-0x7d000 -> key found
# 0x97000-0x9a000 -> key found

tracefiles_sets=engine.run()
for tracefile in tracefiles_sets[0]:
    if phoenixAES.crack(tracefile):
        break
