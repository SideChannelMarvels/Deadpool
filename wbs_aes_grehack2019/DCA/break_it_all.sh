#!/bin/bash

# fine-tuning to some good enough params, to illustrate how fast it would be reproducible on other instances with a different key:
# * 300 traces
# * 200 samples per trace

(
  cd ../target
  sed 's/enc\.c/enc_instrumented.c/' main.c > main_instrumented.c
  sed 's/instr\.c/instr_instrumented.c/' enc.c > enc_instrumented.c
  sed 's/;/;\n/g' instr.c > instr_instrumented.c
  sed -i 's/s\[\([0-9]\+\)\]=\(.*\);/s[\1]=\2;fprintf(stderr, "%c", s[\1]);/' instr_instrumented.c
  gcc -o main_instrumented main_instrumented.c
)

for ((i=0; i<300;i++)); do
  r=$(dd if=/dev/random bs=1 count=16 2>/dev/null|xxd -p)
  c=$(../target/main_instrumented $r 2> trace_tmp)
  mv trace_tmp trace_bits_$(printf "%04i" $i)_${r}_${c}.bin
done

./02_format_traces.py

sed -i "s/nsamples=.*/nsamples=200/" bits_300_9704.config
sed -i "s/ [0-9]\+ / 300 /" bits_300_9704.config
for ((i=0; i<16; i++)); do
  sed -i "s/bytenum=.*/bytenum=$i/" bits_300_9704.config
  daredevil -c bits_300_9704.config | grep -A3 "highest" | grep 1: |cut -c 7-9|tr -d '\n'
done | xxd -r -p
echo

# time: 4.2s including compilation, tracing and DCA
