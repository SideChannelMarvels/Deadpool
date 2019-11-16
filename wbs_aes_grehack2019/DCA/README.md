Usage
-----

Because we've the unrolled source code, we can easily instrument it to print memory accesses:

```
(
  cd ../target
  sed 's/instr\.c/instr_instrumented.c/' main.c > main_instrumented.c
  sed 's/;/;\n/g' instr.c > instr_instrumented.c
  sed -i 's/s\[\([0-9]\+\)\]=\(.*\);/s[\1]=\2;fprintf(stderr, "%c", s[\1]);/' instr_instrumented.c
  gcc -o main_instrumented main_instrumented.c
)
```

Run on random plaintexts:

```
./01_trace_it.sh
./02_format_traces.py
./03_break_it.py
```

Fine-tuning to some good enough params, to illustrate how fast it would be reproducible on other instances with a different key:

* 300 traces
* 200 samples per trace

```
time ./break_it_all.sh 
GH19{AES is FUN}

real    0m4.288s
user    0m6.948s
sys     0m1.187s
```
