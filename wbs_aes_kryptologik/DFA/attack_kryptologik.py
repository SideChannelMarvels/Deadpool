#!/usr/bin/env python3

import sys
import deadpool_dfa_experimental

def processinput(iblock, blocksize):
    p='%0*x' % (2*blocksize, iblock)
    return (None, [p[j*2:(j+1)*2] for j in range(len(p)//2)])

def processoutput(output, blocksize):
    return int(output, 16)

# maxleaf and minleaf are not required, it's just some tuning for a faster attack
lastroundkeys=deadpool_dfa_experimental.AesGetAllRoundKeys(targetbin='./DemoKey_table_encrypt', targetdata='DemoKey_table.bin', goldendata='DemoKey_table.bin.gold',
            processinput=processinput, processoutput=processoutput, verbose=2, maxleaf=0x800, minleaf=0x100, outputbeforelastrounds=True)
print("Possible round keys in the natural order (first ones may be missing):")
print('\n'.join(lastroundkeys[::-1]))
