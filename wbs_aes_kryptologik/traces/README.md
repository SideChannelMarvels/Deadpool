## Usage

## Acquiring one trace with TracerPIN


```bash
Tracer -t sqlite -- ./DemoKey_table_encrypt 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```

Sqlite trace is about 3Mb large.

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph trace-full-info.sqlite &
```

The code is unrolled and the accesses to the table quite linear too. But zooming on the stack reveal something like 7 repetitive structures, the last one being a bit shorter.
It's actually an AES-256 with its 14 rounds.
