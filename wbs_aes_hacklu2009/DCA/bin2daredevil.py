#!/usr/bin/env python

#########################################################################
# bin2daredevil is a script converting binary execution traces          #
# created by Tracer2bin, or other tools using the same format,          #
# into traces compatible with Daredevil along with the required         #
# configuration file.                                                   #
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

# Format:
#trace_defs['dest_keyword']=[source_keyword, struct_input_sample, struct_output_sample, [bits_per_output_sample]]
# struct definitions are those of struct python module
trace_defs={}
# Bytes written on stack:
#trace_defs['stack_w'] =['stack_bytes_w', '<B', '<B', [8]]
# Same, exploded in individual bits:
trace_defs['stack8_w']=['stack_bytes_w', '<B', '<B', [1, 1, 1, 1, 1, 1, 1, 1]]
# Low byte address of data read from data segment:
#trace_defs['addr1_r'] =['mem_addr1_r',   '<B', '<B', [8]]
# Same, exploded in individual bits:
trace_defs['addr8_r'] =['mem_addr1_r',   '<B', '<B', [1, 1, 1, 1, 1, 1, 1, 1]]
# Same, retaining only the lowest bit:
#trace_defs['addrlsb_r'] =['mem_addr1_r',   '<B', '<B', [1]]
# Bytes read from data segment:
#trace_defs['data1_r'] =['mem_data1_r',   '<B', '<B', [8]]
# Same, exploded in individual bits:
#trace_defs['data8_r'] =['mem_data1_r',   '<B', '<B', [1, 1, 1, 1, 1, 1, 1, 1]]

for k,(tag, struct_input_sample, struct_output_sample, list_bits_per_output_sample) in trace_defs.iteritems():
    bytes_per_input_sample = struct.calcsize(struct_input_sample)
    n=len(glob.glob('trace_%s_*' % tag))
    assert n > 0
    traces_meta={}
    min_size=None
    blocksize=None
    for filename in glob.glob('trace_%s_*' % tag):
        i,plain=filename[len('trace_%s_' % tag):-len('.bin')].split('_')
        if not blocksize:
            blocksize=len(plain)/2
        else:
            assert blocksize==len(plain)/2
        filesize = os.path.getsize(filename)
        if not min_size or min_size > filesize:
            min_size = filesize
        traces_meta[int(i)] = [filename, int(plain, 16)]
    ntraces = len(traces_meta)
    nsamples = min_size/bytes_per_input_sample*len(list_bits_per_output_sample)
    with open('%s_%i_%i.trace' % (k, ntraces, nsamples), 'wb') as filetrace,\
         open('%s_%i_%i.plain' % (k, ntraces, nsamples), 'wb') as fileplain:
        for (filename, plain) in traces_meta.itervalues():
            # crypto data
            fileplain.write(('%0*X' % (2*blocksize, plain)).decode('hex'))
            with open(filename, 'rb') as trace:
                for i in range(min_size/bytes_per_input_sample):
                    input_sample,=struct.unpack(struct_input_sample, trace.read(bytes_per_input_sample))
                    for i in list_bits_per_output_sample:
                        # From LSB to MSB
                        output_sample=input_sample & ((1<<i)-1)
                        input_sample=input_sample>>i
                        filetrace.write(struct.pack(struct_output_sample, output_sample))
            os.remove(filename)
    with open('%s_%i_%i.config' % (k, ntraces, nsamples), 'wb') as fileconfig:
        config="""
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
threads=8
order=1
return_type=double
algorithm=AES
position=LUT/AES_AFTER_SBOX
round=0
bitnum=all
bytenum=all
correct_key=0x%s
memory=4G
top=20
""" % (nsamples, \
       '%s_%i_%i.trace' % (k, ntraces, nsamples), ntraces, nsamples, \
       '%s_%i_%i.plain' % (k, ntraces, nsamples), ntraces, blocksize, \
       '14 2b be 0e 2d 22 e4 80 97 49 7d 5f ac 5b 59 26')
        fileconfig.write(config)
