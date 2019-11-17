Usage
-----

Because we've the unrolled source code, we can easily instrument it to print memory accesses:

```
./00_instrument_it.sh
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
