## Usage

## Acquiring one trace with TracerPIN

```bash
Tracer -t sqlite -- ../target/encryptECB -key ../target/key.txt -in <(echo 000102030405060708090a0b0c0d0e0f|xxd -r -p) -out >(xxd -p)
```

Strangely enough I had to kill (SIGTERM) the encryptECB program to end the trace after I got the "Done!" message...

```bash
killall encryptECB
killall encryptECB
killall encryptECB
```

Sqlite trace is about 500Mb large.

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph trace-full-info.sqlite &
```

We can see most AES rounds occur somewhere between 0x474b40 and 0x475df6 with the data beyond 0xc820080000 and the stack between 0xc8201cc000 and 0xc8201db000 (with some data being read only).
