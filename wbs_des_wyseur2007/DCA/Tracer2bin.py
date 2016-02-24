#!/usr/bin/env python

#########################################################################
# Tracer2bin is a script extracting execution traces from whiteboxes.   #
# It requires Tracer (TracerPIN or TracerGrind)                         #
# and outputs binary traces that can be exploited by DPA tools.         #
#                                                                       #
# Copyright (C) 2016                                                    #
# Original author:   Phil Teuwen <phil@teuwen.org>                      #
# Contributors:                                                         #
#                                                                       #
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# any later version.                                                    #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program.  If not, see <http://www.gnu.org/licenses/>. #
#########################################################################

import os
import random
import subprocess

# Number of traces to collect
TRACES=50

TOOL='PIN32'
#TOOL='PIN64'
#TOOL='VALGRIND32'
#TOOL='VALGRIND64'

# Challenge binary
CHALLENGE='../target/wbDES'
# Block size in bytes AES:16, DES:8
BLOCKSIZE=8
def getcipher(output):
    return int(''.join([x for x in output.split('\n') if x.find('OUTPUT')==0][0][10:].split(' ')), 16)
def prepareinput(plaintext):
    p='%0*x' % (2*BLOCKSIZE, plaintext)
    return [p[j*2:(j+1)*2] for j in range(len(p)/2)]

# Temp file
TMPTRACEFILE="trace.tmp%05i" % random.randint(0, 100000)

# Execution address range
# 0 = all
# 1 = filter system libraries
# 2 = filter all but main exec
# 0x400000-0x410000 = trace only specified address range
# Valgrind: reduce at least to 0x400000-0x3ffffff to avoid self-tracing
if TOOL[:3]=='PIN':
    RANGE=2
if TOOL[:8]=='VALGRIND':
    RANGE='0x08000000-0x08100000'

# Stack low address
if TOOL=='PIN32':
    STACK=0xff000000
if TOOL=='PIN64':
    STACK=0x7fff00000000
if TOOL=='VALGRIND32':
    STACK=0xf0000000
if TOOL=='VALGRIND64':
    STACK=0xff0000000
# What to collect
# Format:
# trace_defs['keyword']=[list of 'R'/'W', condition(addr, size, data), extract(addr, size, data)]
trace_defs={}
# Bytes written on stack:
#trace_defs['stack_bytes_w']=[['W'], lambda addr, size, data: addr > STACK and size == 1, lambda addr, size, data: data]
# Low byte address of data read from data segment:
trace_defs['mem_addr1_r']  =[['R'], lambda addr, size, data: addr < STACK and size == 1, lambda addr, size, data: addr & 0xFF]
# Bytes read from data segment:
#trace_defs['mem_data1_r']  =[['R'], lambda addr, size, data: addr < STACK and size == 1, lambda addr, size, data: data]

def tracerpin(tmpfile, i, addr_range, image, plaintext):
    output=subprocess.check_output(['Tracer', '-b', '0', '-c', '0', '-i', '0', '-f', str(addr_range), '-o', tmpfile, '--' , image] + prepareinput(plaintext))
    cipher=getcipher(output)
    tracerpin2bin(tmpfile, i, plaintext, cipher)
    return cipher

def tracerpin2bin(tracefile, n, plaintext, ciphertext):
    with open(tracefile, 'r') as trace:
        for line in iter(trace.readline, ''):
            if len(line) > 2 and (line[1]=='R' or line[1]=='W'):
                mem_mode=line[1]
                mem_addr=int(line[85:99], 16)
                mem_size=int(line[105:107])
                mem_data=int(line[114:].replace(" ",""), 16)
                for k,(tags, condition, extract) in trace_defs.iteritems():
                    if mem_mode in tags and condition(mem_addr, mem_size, mem_data):
                        trace_data[k].append(extract(mem_addr, mem_size, mem_data))
    for k,(tags, test, extract) in trace_defs.iteritems():
        with open('trace_%s_%04i_%0*X_%0*X.bin'
                  % (k, n, 2*BLOCKSIZE, plaintext, 2*BLOCKSIZE, ciphertext), 'wb') as trace:
            trace.write(''.join([chr(x) for x in trace_data[k]]))

def tracergrind(tmpfile, i, addr_range, image, plaintext):
    output=subprocess.check_output(['/usr/local/bin/valgrind', '--quiet', '--trace-children=yes', '--tool=tracergrind', '--filter='+str(addr_range), '--vex-iropt-register-updates=allregs-at-mem-access', '--output='+tmpfile+'.grind', image] + prepareinput(plaintext))
    cipher=getcipher(output)
    output=subprocess.check_output(['texttrace', tmpfile+'.grind', tmpfile])
    os.remove(tmpfile+'.grind')
    tracergrind2bin(tmpfile, i, plaintext, cipher)
    return cipher

def tracergrind2bin(tracefile, n, plaintext, ciphertext):
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
        with open('trace_%s_%04i_%0*X_%0*X.bin'
                  % (k, n, 2*BLOCKSIZE, plaintext, 2*BLOCKSIZE, ciphertext), 'wb') as trace:
            trace.write(''.join([chr(x) for x in trace_data[k]]))

for i in range(TRACES):
    trace_data={}
    for k,v in trace_defs.iteritems():
        trace_data[k]=[]
    plain=random.randint(0, (1<<(8*BLOCKSIZE))-1)
    if TOOL[:3]=='PIN':
        cipher=tracerpin(TMPTRACEFILE, i, RANGE, CHALLENGE, plain)
    elif TOOL[:8]=='VALGRIND':
        cipher=tracergrind(TMPTRACEFILE, i, RANGE, CHALLENGE, plain)
    print '%05i %0*X -> %0*X' % (i, 2*BLOCKSIZE, plain, 2*BLOCKSIZE, cipher)
    os.remove(TMPTRACEFILE)
    del(trace_data)
