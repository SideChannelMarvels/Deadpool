#!/usr/bin/env python

import os
import random
import subprocess

# Number of traces to collect
TRACES=20

# Block size in bytes AES:16, DES:8
BLOCKSIZE=16

# Temp file, no need to change unless you want to run concurrent instances
TMPTRACEFILE='trace.tmp'

# Stack high address
STACKHIGH=0x400000

# What to collect
# Format:
# trace_defs['keyword']=[list of 'R'/'W', condition(addr, size, data), extract(addr, size, data)]
trace_defs={}
# Bytes written on stack:
trace_defs['stack_bytes_w']=[['W'], lambda addr, size, data: addr < STACKHIGH and size == 1, lambda addr, size, data: data]
# Low byte address of data read from data segment:
trace_defs['mem_addr1_r']  =[['R'], lambda addr, size, data: addr > STACKHIGH and size == 1, lambda addr, size, data: addr & 0xFF]
# Bytes read from data segment:
#trace_defs['mem_data1_r']  =[['R'], lambda addr, size, data: addr > STACKHIGH and size == 1, lambda addr, size, data: data]

def tracergrind(tmpfile, i, plaintext):
    ps=""
    for p in "%0*x" % (2*BLOCKSIZE, plaintext):
        if ord(p) >= ord('A'):
            ps+="shift+"
        ps+=p+" "
    output=subprocess.check_output(['./run.sh', ps])
    tracergrind2bin(tmpfile, i, plaintext)

def tracergrind2bin(tracefile, n, plaintext):
    with open(tracefile, 'r') as trace:
        for line in iter(trace.readline, ''):
            if len(line) > 2 and (line[1]=='M'):
                mem_mode=line[line.index('MODE')+6]
                mem_addr=int(line[line.index('START_ADDRESS')+15:line.index('START_ADDRESS')+31], 16)
                mem_size=int(line[line.index('LENGTH')+7:line.index('LENGTH')+9])
                mem_data=int(line[line.index('DATA')+6:].replace(" ",""), 16)
                for k,(tags, condition, extract) in trace_defs.iteritems():
                    if mem_mode in tags and condition(mem_addr, mem_size, mem_data):
                        trace_data[k].append(extract(mem_addr, mem_size, mem_data))
    for k,(tags, test, extract) in trace_defs.iteritems():
        with open('trace_%s_%04i_%0*X.bin'
                  % (k, n, 2*BLOCKSIZE, plaintext), 'wb') as trace:
            trace.write(''.join([chr(x) for x in trace_data[k]]))

for i in range(TRACES):
    trace_data={}
    for k,v in trace_defs.iteritems():
        trace_data[k]=[]
    plain=random.randint(0, (1<<(8*BLOCKSIZE))-1)
    tracergrind(TMPTRACEFILE, i, plain)
    print '%05i %0*X -> ?' % (i, 2*BLOCKSIZE, plain)
    os.remove(TMPTRACEFILE)
    del(trace_data)
