Usage
-----

Instrument the Python script to intercept all new instances of Bits:

```bash
cp ../target/check.py .
patch < check.py.diff
```

Execute a few traces:

```
./01acquire_traces.py
00000 88c057a17954e5f1 -> 0506fda830c8b86e
00001 1771624df07fa709 -> 67bf58a0bd934ffe
00002 0ba6ad4b3f4230b3 -> 9da45ed8f8fbad75
00003 aa43177d69b2e21b -> c5ed17f8aafe8463
00004 6c739e7735004227 -> f8ae9725cb63efa3
00005 e205e811a3fbf8ec -> a0f24392903a078f
00006 b3d52e25af5954de -> f8e1b76d40672319
...
```

Convert the traces to Daredevil format:

```
./02format_traces.py
```

Execute the differential analysis on the converted traces:

```bash
daredevil -c bits_25_1824.config |grep rank
Best bit: 0 rank: 0.               1    0x2d     283
Best bit: 0 rank: 0.               1    0x18     275
Best bit: 0 rank: 0.               1    0x3d     267
Best bit: 1 rank: 0.               1    0x35     258
Best bit: 0 rank: 0.               1    0x39     251
Best bit: 0 rank: 0.               1    0x1d     243
Best bit: 0 rank: 0.               1    0x28     235
Best bit: 0 rank: 0.               1    0x27     227
```
