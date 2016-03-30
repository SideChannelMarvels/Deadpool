#########################################################################
# deadpool_dca is a Python library to help extracting execution traces  #
# from whiteboxes and convert them into traces compatible with          #
# Daredevil or Riscure Inspector                                        #
#                                                                       #
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
import glob
import struct
import random
import subprocess

def processinput(iblock, blocksize):
    """processinput() helper function
   iblock: int representation of one input block
   blocksize: int (8 for DES, 16 for AES)
   returns a list of strings to be used as args for the target
   default processinput(): returns one string containing the block in hex
"""
    return ['%0*x' % (2*blocksize, iblock)]

def processoutput(output, blocksize):
    """processoutput() helper function
   output: string, textual output of the target
   blocksize: int (8 for DES, 16 for AES)
   returns a int, supposed to be the data block outputted by the target
   default processouput(): expects the output to be directly the block in hex
"""
    return int(output, 16)

class ARCH:
    i386  = 0
    amd64 = 1

class FILTER:
    #['keyword', list of 'R'/'W', condition((stacklow, stackhigh), addr, size, data), extract(addr, size, data), pack_fmt]
    # Bytes written on stack:
    stack_w1 =['stack_w1', ['W'], lambda stack_range, addr, size, data: stack_range[0] <= addr <= stack_range[1] and size == 1, lambda addr, size, data: data, '<B']
    # Low byte address of data read from data segment:
    mem_addr_rw1   =['mem_addr_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: addr & 0xFF, '<B']
    # Bytes read from data segment:
    mem_data_rw1   =['mem_data_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: data, '<B']

class Tracer(object):
    def __init__(self, target,
                   processinput,
                   processoutput,
                   arch,
                   blocksize,
                   tmptracefile,
                   addr_range,
                   stack_range,
                   filters,
                   tolerate_error,
                   debug):
        self.target=target
        self.processinput=processinput
        self.processoutput=processoutput
        self.arch=arch
        self.blocksize=blocksize
        if tmptracefile == 'default':
            self.tmptracefile="trace.tmp%05i" % random.randint(0, 100000)
        else:
            self.tmptracefile=tmptracefile
        self.addr_range=addr_range
        self.stack_range=stack_range
        if self.stack_range != 'default':
            self.stack_range=(int(stack_range[:stack_range.index('-')], 16), int(stack_range[stack_range.index('-')+1:], 16))
        if filters == 'default':
            self.filters=[FILTER.stack_w1, FILTER.mem_addr_rw1, FILTER.mem_data_rw1]
        else:
            self.filters=filters
        self.tolerate_error=tolerate_error
        self.debug=debug

    def run(self, n, verbose=True):
        for i in range(n):
            iblock=random.randint(0, (1<<(8*self.blocksize))-1)
            oblock=self.get_trace(i, iblock)
            if verbose:
                print '%05i %0*X -> %0*X' % (i, 2*self.blocksize, iblock, 2*self.blocksize, oblock)

    def _trace_init(self, n, iblock, oblock):
        self._trace_meta=(n, iblock, oblock)
        self._trace_data={}
        for f in self.filters:
            self._trace_data[f[0]]=[]

    def _trace_dump(self):
        n, iblock, oblock = self._trace_meta
        for (k, tags, condition, extract, pack_fmt) in self.filters:
            with open('trace_%s_%04i_%0*X_%0*X.bin'
                      % (k, n, 2*self.blocksize, iblock, 2*self.blocksize, oblock), 'wb') as trace:
                trace.write(''.join([struct.pack(pack_fmt, x) for x in self._trace_data[k]]))
        del(self._trace_data)

class TracerPIN(Tracer):
    def __init__(self, target,
                   processinput=processinput,
                   processoutput=processoutput,
                   arch=ARCH.amd64,
                   blocksize=16,
                   tmptracefile='default',
                   addr_range='default',
                   stack_range='default',
                   filters='default',
                   tolerate_error=False,
                   debug=False):
        super(TracerPIN, self).__init__(target, processinput, processoutput, arch, blocksize, tmptracefile, addr_range, stack_range, filters, tolerate_error, debug)
        # Execution address range
        # 0 = all
        # 1 = filter system libraries
        # 2 = filter all but main exec
        # 0x400000-0x410000 = trace only specified address range
        if self.addr_range == 'default':
            self.addr_range=2
        if stack_range == 'default':
            if self.arch==ARCH.i386:
                self.stack_range =(0xff000000, 0xffffffff)
            elif self.arch==ARCH.amd64:
                self.stack_range =(0x7fff00000000, 0x7fffffffffff)

    def get_trace(self, n, iblock):
        cmd_list=['Tracer', '-q', '1', '-b', '0', '-c', '0', '-i', '0', '-f', str(self.addr_range), '-o', self.tmptracefile, '--', self.target] + self.processinput(iblock, self.blocksize)
        if self.debug:
            print ' '.join(cmd_list)
        if self.tolerate_error:
            output=subprocess.check_output(' '.join(cmd_list) + '; exit 0', shell=True)
        else:
            output=subprocess.check_output(cmd_list)
        oblock=self.processoutput(output, self.blocksize)
        self._trace_init(n, iblock, oblock)
        with open(self.tmptracefile, 'r') as trace:
            for line in iter(trace.readline, ''):
                if len(line) > 2 and (line[1]=='R' or line[1]=='W'):
                    mem_mode=line[1]
                    mem_addr=int(line[85:99], 16)
                    mem_size=int(line[105:107])
                    mem_data=int(line[114:].replace(" ",""), 16)
                    for (k, tags, condition, extract, pack_fmt) in self.filters:
                        if mem_mode in tags and condition(self.stack_range, mem_addr, mem_size, mem_data):
                            self._trace_data[k].append(extract(mem_addr, mem_size, mem_data))
        self._trace_dump()
        os.remove(self.tmptracefile)
        return oblock

class TracerGrind(Tracer):
    def __init__(self, target,
                   processinput=processinput,
                   processoutput=processoutput,
                   arch=ARCH.amd64,
                   blocksize=16,
                   tmptracefile='default',
                   addr_range='default',
                   stack_range='default',
                   filters='default',
                   tolerate_error=False,
                   debug=False):
        super(TracerGrind, self).__init__(target, processinput, processoutput, arch, blocksize, tmptracefile, addr_range, stack_range, filters, tolerate_error, debug)
        # Execution address range
        # Valgrind: reduce at least to 0x400000-0x3ffffff to avoid self-tracing
        if addr_range == 'default':
            self.addr_range='0x400000-0x3ffffff'
        if stack_range == 'default':
            if self.arch==ARCH.i386:
                self.stack_range =(0xf0000000, 0xffffffff)
            if self.arch==ARCH.amd64:
                self.stack_range =(0xff0000000, 0xfffffffff)

    def get_trace(self, n, iblock):
        cmd_list=['valgrind', '--quiet', '--trace-children=yes', '--tool=tracergrind', '--filter='+str(self.addr_range), '--vex-iropt-register-updates=allregs-at-mem-access', '--output='+self.tmptracefile+'.grind', self.target] + self.processinput(iblock, self.blocksize)
        if self.debug:
            print ' '.join(cmd_list)
        if self.tolerate_error:
            output=subprocess.check_output(' '.join(cmd_list) + '; exit 0', shell=True)
        else:
            output=subprocess.check_output(cmd_list)
        oblock=self.processoutput(output, self.blocksize)
        output=subprocess.check_output("texttrace %s >(grep '^.M' > %s)" % (self.tmptracefile+'.grind', self.tmptracefile), shell=True, executable='/bin/bash')
        os.remove(self.tmptracefile+'.grind')
        self._trace_init(n, iblock, oblock)
        with open(self.tmptracefile, 'r') as trace:
            for line in iter(trace.readline, ''):
                mem_mode=line[line.index('MODE')+6]
                mem_addr=int(line[line.index('START_ADDRESS')+15:line.index('START_ADDRESS')+31], 16)
                mem_size=int(line[line.index('LENGTH')+7:line.index('LENGTH')+9])
                mem_data=int(line[line.index('DATA')+6:].replace(" ",""), 16)
                for (k, tags, condition, extract, pack_fmt) in self.filters:
                    if mem_mode in tags and condition(self.stack_range, mem_addr, mem_size, mem_data):
                        self._trace_data[k].append(extract(mem_addr, mem_size, mem_data))
        self._trace_dump()
        os.remove(self.tmptracefile)
        return oblock

def serializechars(s, _out={}):
    """Replaces each byte of the string by 8 bytes representing the bits, starting with their LSB
"""
    # Memoization using mutable dict
    if not _out:
        for b in range(256):
            n=b
            o=''
            for i in range(8):
                o+=chr(n&1)
                n=n>>1
            _out[chr(b)]=o
    return ''.join(_out[x] for x in s)
