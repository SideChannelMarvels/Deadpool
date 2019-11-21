#!/bin/bash

(
  cd ../target
  tar xf 1573854749.4296393_e6b19a0f2ddea947e2166ddf9c7fa852e93cd3f4.tar.gz
  sed 's/enc\.c/enc_instrumented.c/' main.c > main_instrumented.c
  sed 's/instr\.c/instr_instrumented.c/' enc.c > enc_instrumented.c
  sed 's/;/;\n/g' instr.c > instr_instrumented.c
  sed -i 's/s\[\([0-9]\+\)\]=\(.*\);/s[\1]=\2;fprintf(stderr, "%c", s[\1]);/' instr_instrumented.c
  gcc -o main_instrumented main_instrumented.c
)
