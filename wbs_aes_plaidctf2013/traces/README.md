## Usage

## Acquiring one trace with TracerPIN

```bash
dd if=/dev/random of=foo bs=64 count=1
Tracer -t sqlite -- ../target/drmless -f -E foo
```

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph trace-full-info.sqlite &
```

Instructions of the white-box are 0x8054060 to 0x82a2279 and stack at 0xffffa000.
