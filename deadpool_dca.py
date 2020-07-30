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
import math
import re

# Adapt paths to your setup if needed:
tracergrind_exec='/usr/local/bin/valgrind'
tracerpin_exec='/usr/local/bin/Tracer'

def processinput(iblock, blocksize):
    """processinput() helper function
   iblock: int representation of one input block
   blocksize: int (8 for DES, 16 for AES)
   returns: (bytes to be used as target stdin, a list of strings to be used as args for the target)
   default processinput(): returns (None, one string containing the block in hex)
   return (None, None) if input can't be injected via stdin or args
"""
    return (None, ['%0*x' % (2*blocksize, iblock)])
# Example to provide input as raw chars on stdin:
#    return (bytes.fromhex('%0*x' % (2*blocksize, iblock)), None)

def processoutput(output, blocksize):
    """processoutput() helper function
   output: string, textual output of the target
   blocksize: int (8 for DES, 16 for AES)
   returns a int, supposed to be the data block outputted by the target
   default processouput(): expects the output to be directly the block in hex
"""
    # return None if there is no output available
    return int(output, 16)

class ARCH:
    i386  = 0
    amd64 = 1

class Filter:
    def __init__(self, keyword, modes, condition, extract, extract_fmt):
        self.keyword=keyword
        self.modes=modes
        self.condition=condition
        self.extract=extract
        self.extract_fmt=extract_fmt
        self.record_info=False
    def __str__(self):
        return self.keyword

class DefaultFilters:
    # Bytes written on stack:
    stack_w1      =Filter('stack_w1', ['W'], lambda stack_range, addr, size, data: stack_range[0] <= addr <= stack_range[1] and size == 1, lambda addr, size, data: data, '<B')
    stack_w4      =Filter('stack_w4', ['W'], lambda stack_range, addr, size, data: stack_range[0] <= addr <= stack_range[1] and size == 4, lambda addr, size, data: data, '<I')
    # Low byte(s) address of data read from data segment:
    mem_addr1_rw1 =Filter('mem_addr1_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: addr & 0xFF, '<B')
    mem_addr1_rw4 =Filter('mem_addr1_rw4', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 4, lambda addr, size, data: addr & 0xFF, '<B')
    mem_addr2_rw1 =Filter('mem_addr2_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: addr & 0xFFFF, '<H')
    # Bytes read from data segment:
    mem_data_rw1  =Filter('mem_data_rw1', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 1, lambda addr, size, data: data, '<B')
    mem_data_rw4  =Filter('mem_data_rw4', ['R', 'W'], lambda stack_range, addr, size, data: (addr < stack_range[0] or addr > stack_range[1]) and size == 4, lambda addr, size, data: data, '<I')

DEFAULT_FILTERS=[DefaultFilters.stack_w1, DefaultFilters.mem_addr1_rw1, DefaultFilters.mem_data_rw1]

def findbin(keyword):
    n=len(glob.glob('trace_%s_*.bin' % keyword))
    assert n > 0
    traces_meta={}
    min_size=None
    iblock_available=True
    oblock_available=True
    for filename in glob.glob('trace_%s_*.bin' % keyword):
        i,iblock,oblock=filename[len('trace_%s_' % keyword):-len('.bin')].split('_')
        if iblock!='na':
            blocksize=len(iblock)/2
            assert iblock_available==True
        else:
            iblock_available=False
        if oblock!='na':
            if not iblock_available:
                blocksize=len(oblock)/2
            else:
                assert blocksize==len(oblock)/2
            assert oblock_available==True
        else:
            oblock_available=False
        filesize = os.path.getsize(filename)
        if not min_size or min_size > filesize:
            min_size = filesize
        traces_meta[filename] = [iblock, oblock]
    ntraces = len(traces_meta)
    nsamples = min_size*8
    return (ntraces, nsamples, min_size, blocksize, traces_meta, iblock_available, oblock_available)

def bin2daredevil(keyword=None, keywords=None, delete_bin=True, config=None, configs=None):
    assert keyword is None or keywords is None
    if keyword is not None:
        keywords=[keyword]
    if keywords is None:
        keywords=DEFAULT_FILTERS
    assert config is None or configs is None
    if config is not None:
        configs={'':config}
    if configs is None:
        configs={'':{}}
    for keyword in keywords:
        ntraces, nsamples, min_size, blocksize, traces_meta, iblock_available, oblock_available = findbin(keyword)
        with open('%s_%i_%i.trace' % (keyword, ntraces, nsamples), 'wb') as filetrace,\
             open('%s_%i_%i.input' % (keyword, ntraces, nsamples), 'wb') as fileinput,\
             open('%s_%i_%i.output' % (keyword, ntraces, nsamples), 'wb') as fileoutput:
            for filename, (iblock, oblock) in traces_meta.iteritems():
                if iblock_available:
                    fileinput.write(iblock.decode('hex'))
                if oblock_available:
                    fileoutput.write(oblock.decode('hex'))
                with open(filename, 'rb') as trace:
                    filetrace.write(serializechars(trace.read(min_size)))
                if delete_bin:
                    os.remove(filename)
        for configname, config in configs.iteritems():
            if 'threads' not in config:
                config['threads']='8'
            if 'algorithm' not in config:
                config['algorithm']='AES'
            if 'position' not in config:
                config['position']='LUT/AES_AFTER_SBOX'
            if 'des_switch' in config:
                config['position']+='\ndes_switch='+config['des_switch']
            if 'guess' not in config:
                config['guess']='input'
            if 'bytenum' not in config:
                config['bytenum']='all'
            if 'bitnum' not in config:
                config['bitnum']='all'
            if 'memory' not in config:
                config['memory']='4G'
            if 'top' not in config:
                config['top']='20'
            if 'comment_correct_key' not in config:
                if 'correct_key' in config:
                    config['comment_correct_key']=''
                else:
                    config['comment_correct_key']='#'
                    config['correct_key']='0x000102030405060708090a0b0c0d0e0f'
            if config['correct_key'][:2] != "0x":
                config['correct_key'] = "0x" + config['correct_key']
            if configname:
                configname+='.'
            content="""
[Traces]
files=1
trace_type=i
transpose=true
index=0
nsamples=%i
trace=%s %i %i

[Guesses]
files=1
guess_type=u
transpose=true
guess=%s %i %i

[General]
threads=%s
order=1
return_type=double
algorithm=%s
position=%s
round=0
bitnum=%s
bytenum=%s
%scorrect_key=%s
memory=%s
top=%s
""" % (nsamples, \
   '%s_%i_%i.trace' % (keyword, ntraces, nsamples), ntraces, nsamples, \
   '%s_%i_%i.%s' % (keyword, ntraces, nsamples, config['guess']), ntraces, blocksize, \
   config['threads'], config['algorithm'], config['position'], \
   config['bitnum'], config['bytenum'], \
   config['comment_correct_key'], config['correct_key'], \
   config['memory'], config['top'])
            with open('%s_%i_%i.%sconfig' % (keyword, ntraces, nsamples, configname), 'wb') as fileconfig:
                fileconfig.write(content)

def bin2trs(keyword=None, keywords=None, delete_bin=True):
    assert keyword is None or keywords is None
    if keyword is not None:
        keywords=[keyword]
    if keywords is None:
        keywords=DEFAULT_FILTERS
    for keyword in keywords:
        ntraces, nsamples, min_size, blocksize, traces_meta, iblock_available, oblock_available = findbin(keyword)
        with open('%s_%i_%i.trs' % (keyword, ntraces, nsamples), 'wb') as trs:
            trs.write('\x41\x04' + struct.pack('<I', ntraces))
            trs.write('\x42\x04' + struct.pack('<I', nsamples))
            # Sample Coding
            #   bit 8-6: 000
            #   bit 5:   integer(0) or float(1)
            #   bit 4-1: sample length in bytes (1,2,4)
            trs.write('\x43\x01' + chr(struct.calcsize('<B')))
            # Length of crypto data
            trs.write('\x44\x02' + struct.pack('<H', blocksize*iblock_available+blocksize*oblock_available))
            # End of header
            trs.write('\x5F\x00')
            for filename, (iblock, oblock) in traces_meta.iteritems():
                if iblock_available:
                    trs.write(iblock.decode('hex'))
                if oblock_available:
                    trs.write(oblock.decode('hex'))
                with open(filename, 'rb') as trace:
                    trs.write(serializechars(trace.read(min_size)))
                if delete_bin:
                    os.remove(filename)

def sample2event(sample, filtr, target):
    # returns (event number, optional list of details (mem_mode, item, ins_addr, mem_addr, mem_size, mem_data, src_line_info))
    # assuming serialized samples
    ievent=int(math.ceil(float(sample)/struct.calcsize(filtr.extract_fmt)/8))
    # Let's see if we've more info...
    eventlist=[]
    for filename in glob.glob('trace_%s_*.info' % filtr.keyword):
        with open(filename) as info:
            for i, line in enumerate(info):
                if i+1 == ievent:
                    mem_mode, item, ins_addr, mem_addr, mem_size, mem_data = line.split()
                    item, ins_addr, mem_addr, mem_size, mem_data=int(item), int(ins_addr, 16), int(mem_addr, 16), int(mem_size), int(mem_data, 16)
                    try:
                        output=subprocess.check_output(['addr2line', '-e', target, '0x%X'%ins_addr])
                        output=output.split('/')[-1].strip()
                    except:
                        output=''
                    eventlist.append((mem_mode, item, ins_addr, mem_addr, mem_size, mem_data, output))
                elif i > ievent:
                    break
    return (ievent, eventlist)

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
                   shell,
                   debug):
        self.target=target.split()
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
            self.filters=DEFAULT_FILTERS
        else:
            self.filters=filters
        self.tolerate_error=tolerate_error
        self.shell=shell
        self.debug=debug

    def run(self, n, verbose=True):
        self.verbose=verbose
        for i in range(n):
            iblock=random.randint(0, (1<<(8*self.blocksize))-1)
            oblock=self.get_trace(i, iblock)

    def _exec(self, cmd_list, input_stdin, debug=None):
        if debug is None:
            debug=self.debug
        if debug:
            print ' '.join(cmd_list)
        if self.tolerate_error:
            proc = subprocess.Popen(' '.join(cmd_list) + '; exit 0', stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, executable='/bin/bash')
        elif self.shell:
            proc = subprocess.Popen(' '.join(cmd_list), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, executable='/bin/bash')
        else:
            proc = subprocess.Popen(cmd_list, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        output, errs = proc.communicate(input=input_stdin)
        if debug:
            print output
        return output

    def _trace_init(self, n, iblock, oblock):
        self._trace_meta=(n, iblock, oblock)
        self._trace_data={}
        self._trace_info={}
        for f in self.filters:
            self._trace_data[f.keyword]=[]
            self._trace_info[f.keyword]=[]

    def _trace_dump(self):
        n, iblock, oblock = self._trace_meta
        if iblock is not None:
            iblockstr='%0*X' % (2*self.blocksize, iblock)
        else:
            iblockstr='na'
        if oblock is not None:
            oblockstr='%0*X' % (2*self.blocksize, oblock)
        else:
            oblockstr='na'
        for f in self.filters:
            with open('trace_%s_%04i_%s_%s.bin'
                  % (f.keyword, n, iblockstr, oblockstr), 'wb') as trace:
                trace.write(''.join([struct.pack(f.extract_fmt, x) for x in self._trace_data[f.keyword]]))
            if f.record_info:
                with open('trace_%s_%s_%s.info'
                      % (f.keyword, iblockstr, oblockstr), 'wb') as trace:
                    for mem_mode, item, ins_addr, mem_addr, mem_size, mem_data in self._trace_info[f.keyword]:
                        trace.write("[%s] %7i %16X %16X %2i %0*X\n" % (mem_mode, item, ins_addr, mem_addr, mem_size, 2*mem_size, mem_data))
                f.record_info=False
        del(self._trace_data)
        del(self._trace_info)
        if self.verbose:
            print '%05i %s -> %s' % (n, iblockstr, oblockstr)

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
                   shell=False,
                   debug=False,
                   record_info=True):
        super(TracerPIN, self).__init__(target, processinput, processoutput, arch, blocksize, tmptracefile, addr_range, stack_range, filters, tolerate_error, shell, debug)
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
        if record_info:
            for f in self.filters:
                f.record_info=True
    def get_trace(self, n, iblock):
        processed_input=self.processinput(iblock, self.blocksize)
        input_stdin, input_args = processed_input
        if input_stdin is None:
            input_stdin=b''
        if input_args is None:
            input_args=[]
        cmd_list=[tracerpin_exec, '-q', '1', '-b', '0', '-c', '0', '-i', '0', '-f', str(self.addr_range), '-o', self.tmptracefile, '--'] + self.target + input_args
        output=self._exec(cmd_list, input_stdin)
        oblock=self.processoutput(output, self.blocksize)
        self._trace_init(n, iblock, oblock)
        with open(self.tmptracefile, 'r') as trace:
            for line in iter(trace.readline, ''):
                if len(line) > 2 and (line[1]=='R' or line[1]=='W'):
                    m=re.search(r'\[(.)\] *([0-9]+)(0x[0-9a-fA-F]+) *(0x[0-9a-fA-F]+) *size= *([0-9]+) *value= *(.*)', line)
                    assert m is not None
                    mem_mode=m.group(1)
                    item=int(m.group(2))
                    ins_addr=int(m.group(3), 16)
                    mem_addr=int(m.group(4), 16)
                    mem_size=int(m.group(5), 16)
                    mem_data=int(m.group(6).replace(" ",""), 16)
                    for f in self.filters:
                        if mem_mode in f.modes and f.condition(self.stack_range, mem_addr, mem_size, mem_data):
                            if f.record_info:
                                self._trace_info[f.keyword].append((mem_mode, item, ins_addr, mem_addr, mem_size, mem_data))
                            self._trace_data[f.keyword].append(f.extract(mem_addr, mem_size, mem_data))
        self._trace_dump()
        if not self.debug:
            os.remove(self.tmptracefile)
        return oblock

    def run_once(self, iblock=None, tracefile=None):
        if iblock is None:
            iblock=random.randint(0, (1<<(8*self.blocksize))-1)
        if tracefile is None:
            tracefile = self.tmptracefile
        processed_input=self.processinput(iblock, self.blocksize)
        input_stdin, input_args = processed_input
        if input_stdin is None:
            input_stdin=b''
        if input_args is None:
            input_args=[]
        cmd_list=[tracerpin_exec, '-f', str(self.addr_range), '-o', tracefile, '--'] + self.target + input_args
        output=self._exec(cmd_list, input_stdin, debug=True)

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
                   shell=False,
                   debug=False,
                   record_info=False):
        super(TracerGrind, self).__init__(target, processinput, processoutput, arch, blocksize, tmptracefile, addr_range, stack_range, filters, tolerate_error, shell, debug)
        # Execution address range
        # Valgrind: reduce at least to 0x400000-0x3ffffff to avoid self-tracing
        if addr_range == 'default':
            self.addr_range='0x400000-0x3ffffff,' + os.path.realpath(target)
        if stack_range == 'default':
            if self.arch==ARCH.i386:
                self.stack_range =(0xf0000000, 0xffffffff)
            if self.arch==ARCH.amd64:
                self.stack_range =(0xff0000000, 0xfffffffff)
        if record_info:
            raise ValueError("Sorry, option not yet supported!")

    def get_trace(self, n, iblock):
        processed_input=self.processinput(iblock, self.blocksize)
        input_stdin, input_args = processed_input
        if input_stdin is None:
            input_stdin=b''
        if input_args is None:
            input_args=[]
        cmd_list=[tracergrind_exec, '--quiet', '--trace-children=yes', '--tool=tracergrind', '--filter='+str(self.addr_range), '--vex-iropt-register-updates=allregs-at-mem-access', '--output='+self.tmptracefile+'.grind'] + self.target + input_args
        output=self._exec(cmd_list, input_stdin)
        oblock=self.processoutput(output, self.blocksize)
        output=subprocess.check_output("texttrace %s >(grep '^.M' > %s)" % (self.tmptracefile+'.grind', self.tmptracefile), shell=True, executable='/bin/bash')
        if not self.debug:
            os.remove(self.tmptracefile+'.grind')
        self._trace_init(n, iblock, oblock)
        with open(self.tmptracefile, 'r') as trace:
            for line in iter(trace.readline, ''):
                mem_mode=line[line.index('MODE')+6]
                mem_addr=int(line[line.index('START_ADDRESS')+15:line.index('START_ADDRESS')+31], 16)
                mem_size=int(line[line.index('LENGTH')+7:line.index('LENGTH')+10])
                mem_data=int(line[line.index('DATA')+6:].replace(" ",""), 16)
                for f in self.filters:
                    if mem_mode in f.modes and f.condition(self.stack_range, mem_addr, mem_size, mem_data):
                        self._trace_data[f.keyword].append(f.extract(mem_addr, mem_size, mem_data))
        self._trace_dump()
        if not self.debug:
            os.remove(self.tmptracefile)
        return oblock

    def run_once(self, iblock=None, tracefile=None):
        if iblock is None:
            iblock=random.randint(0, (1<<(8*self.blocksize))-1)
        if tracefile is None:
            tracefile = self.tmptracefile
        processed_input=self.processinput(iblock, self.blocksize)
        input_stdin, input_args = processed_input
        if input_stdin is None:
            input_stdin=b''
        if input_args is None:
            input_args=[]
        cmd_list=[tracergrind_exec, '--trace-children=yes', '--tool=tracergrind', '--filter='+str(self.addr_range), '--vex-iropt-register-updates=allregs-at-mem-access', '--output='+tracefile+'.grind'] + self.target + input_args
        output=self._exec(cmd_list, input_stdin, debug=True)
        output=subprocess.check_output("texttrace %s %s" % (tracefile+'.grind',tracefile), shell=True)
        os.remove(tracefile+'.grind')

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
