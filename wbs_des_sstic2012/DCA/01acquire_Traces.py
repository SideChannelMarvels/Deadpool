#!/usr/bin/env python

import sys
import random
import check

keyword='bits'

# Just to clean a bit the traces...
def remove_duplicates(values):
    output = []
    seen = set()
    for value in values:
        if value not in seen:
            output.append(value)
            seen.add(value)
    return output

for i in range(25):
    check.trace=[]
    M = check.Bits(random.getrandbits(64), 64)
    C = check.WT._cipher(M, 1)
    plain=hex(M).encode('hex')
    cipher=hex(C).encode('hex')
    print "%05i %s -> %s" % (i, plain, cipher)
    filteredtrace=remove_duplicates([(s, v) for s,v in check.trace if s == 96])
    with open('trace_%s_%04i_%s_%s.bin' % (keyword, i, plain, cipher), 'wb') as trace:
        trace.write(''.join(["%0*x" % (s/4, v) for s,v in filteredtrace]).decode('hex'))
    del(check.trace)
