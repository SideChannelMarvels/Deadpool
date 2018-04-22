## Usage

## Acquiring one trace with TracerPIN

```bash
Tracer -t sqlite -- ../target/wb_challenge 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
```

Sqlite trace is about 2.5Mb large.

## Acquiring one trace with TracerGrind

```bash
valgrind --tool=tracergrind --filter=0x000000-0x600000 --output=ches2016.trace ../target/wb_challenge 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
sqlitetrace ches2016.trace trace-full-info.sqlite
```

## Visualizing

Just fire tracegraph and load the sqlite trace:

```
tracegraph trace-full-info.sqlite &
```

On the full graph, instructions and table reads are pretty linear.
Instructions range is from 0x108000 to 0x130000.

On the stack, it's still hard to distinguish individual rounds.
Still, one can observe 72 smaller units, which correspond to the 9 AES-128 rounds (minus the last round without MixCols and typically merged in the previous one in white-box setups).
