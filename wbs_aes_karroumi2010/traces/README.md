## Usage

## Acquiring one trace with TracerPIN on the Boost 1.58 version


```bash
Tracer -t sqlite -- ../target/main --load-tables tables_karroumi_extenc.tbl --extEnc=1 --input-files <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) --out-file >(xxd -p)
```

Sqlite trace is about 1Gb large.

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph trace-full-info.sqlite &
```

We can see most AES rounds occur somewhere between 0x4676d0 and 0x468eed with the stack beyond 0x7ffffff68000.

## Acquiring and visualiszing one trace with TracerGrind on the Boost 1.62 version


```bash
valgrind --tool=tracergrind --output=trace.grind --filter=0x000000-0x400000 ./main --load-tables tables_karroumi_noextenc.tbl --input-files <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) --out-file >(xxd -p)
sqlitetrace trace.grind trace.sqlite
tracegraph trace.sqlite &
```

We can see most AES rounds occur somewhere between 0x178000 and 0x17b700 with the stack beyond 0xffef6a000.

Note that if you're using other Boost versions, address range will vary.
