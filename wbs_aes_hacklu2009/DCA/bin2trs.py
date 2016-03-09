#!/usr/bin/env python

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
    with open('trs_%s_%i_%i.trs' % (k, n, min_size), 'wb') as trs:
        # Nr of traces
        trs.write('\x41\x04' + struct.pack('<I', n))
        # Nr of samples
        trs.write('\x42\x04' + struct.pack('<I', min_size/bytes_per_input_sample*len(list_bits_per_output_sample)))
        # Sample Coding
        #   bit 8-6: 000
        #   bit 5:   integer(0) or float(1)
        #   bit 4-1: sample length in bytes (1,2,4)
        trs.write('\x43\x01' + chr(struct.calcsize(struct_output_sample)))
        # Length of crypto data
        trs.write('\x44\x02' + struct.pack('<H', blocksize))
        # End of header
        trs.write('\x5F\x00')
        for (filename, plain) in traces_meta.itervalues():
            # crypto data
            trs.write(('%0*X' % (2*blocksize, plain)).decode('hex'))
            with open(filename, 'rb') as trace:
                for i in range(min_size/bytes_per_input_sample):
                    input_sample,=struct.unpack(struct_input_sample, trace.read(bytes_per_input_sample))
                    for i in list_bits_per_output_sample:
                        # From LSB to MSB
                        output_sample=input_sample & ((1<<i)-1)
                        input_sample=input_sample>>i
                        trs.write(struct.pack(struct_output_sample, output_sample))
            os.remove(filename)
