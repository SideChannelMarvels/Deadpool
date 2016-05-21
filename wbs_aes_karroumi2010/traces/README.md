## Usage

## Acquiring one trace with TracerPIN


```bash
Tracer -t sqlite -- ../target/main --load-tables tables_karroumi_extenc.tbl --extEnc=1 --input-files <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) --out-file >(xxd -p)
```

Sqlite trace is about 1Gb large.

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph &
```

We can see most AES rounds occur somewhere between 0x4676d0 and 0x468eed with the stack beyond 0x7ffffff68000.

Note that if you're using the version compiled for Boost 1.55, address range will vary.
