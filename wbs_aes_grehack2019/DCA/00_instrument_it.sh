#!/bin/bash

(
  cd ../target
  sed 's/instr\.c/instr_instrumented.c/' main.c > main_instrumented.c
  sed 's/;/;\n/g' instr.c > instr_instrumented.c
  sed -i 's/s\[\([0-9]\+\)\]=\(.*\);/s[\1]=\2;fprintf(stderr, "%c", s[\1]);/' instr_instrumented.c
  gcc -o main_instrumented main_instrumented.c
)
