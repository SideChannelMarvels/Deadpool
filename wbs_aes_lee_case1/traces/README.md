## Usage

## Acquiring one trace with TracerPIN

```bash
ln -s ../target/Tab.bin
Tracer -t sqlite -- ../target/WB_LEE_CASE1_Client $(echo -n testtesttesttest|xxd -p|sed 's/\(..\)/\1 /g')
```

Sqlite trace is about 315Mb large.

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph trace-full-info.sqlite &
```
