## Usage

## Acquiring one trace with TracerPIN

```bash
ln -s ../target/constr.txt
ln -s ../target/constr.key
export GOGC=off
Tracer -t sqlite -o localextenc.sqlite -- ../target/encrypt_localextenc -block 000102030405060708090a0b0c0d0e0f
```

Strangely enough I had to kill (SIGTERM) the encryptECB program to end the trace after I got the ciphertext...

```bash
killall encryptECB
killall encryptECB
killall encryptECB
```

Sqlite trace is about 7300Mb large!

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph localextenc.sqlite &
```

