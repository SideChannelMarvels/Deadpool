#!/bin/bash

for ((i=0; i<500;i++)); do
  r=$(dd if=/dev/random bs=1 count=16 2>/dev/null|xxd -p)
  c=$(../target/main_instrumented $r 2> trace_tmp)
  mv trace_tmp trace_bits_$(printf "%04i" $i)_${r}_${c}.bin
done
