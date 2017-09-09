#!/usr/bin/env python3

ntraces=300
nsamples=6095
with open('traces.trs.traces', 'rb') as trs:
    for i in range(ntraces):
        with open("traces.trs.traces_%04i" % i, 'wb') as out:
            out.write(trs.read(nsamples*4))
