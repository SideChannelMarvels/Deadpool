## Usage

## Acquiring one trace with TracerPIN


```bash
Tracer -t sqlite -- ../target/main --load-tables tables_karroumi_noextenc.tbl --input-files <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) --out-file >(xxd -p)
```

Sqlite trace is about 1Gb large.

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph &
```
