Usage
-----

Using Tracer to acquire execution traces, here recording the lowest byte of each address from which data is being read:

```bash
./Tracer2bin.py
```

By default the script is using TracerPIN but if you want you can change it to use TracerGrind:

```diff
-TOOL='PIN32'
+TOOL='VALGRIND32'
```

Converting traces for Daredevil:

```bash
./bin2daredevil.py
```

Executing the differential analysis on the converted traces:

```bash
daredevil -c addr8_r_50_72072.config
```

Here we already provided the correct DES key in the configuration and the tool returns the rank of the correct 6-bit round key segments amongst all the key candidates.
Typically 50 traces are enough to rank the correct ones in first position.

To demonstrate an attack without knowing the key, edit addr8_r_50_72072.config:

* Comment out the correct_key
* Choose which key segment to break, e.g.: bytenum=0

At this moment Daredevil doesn't attack yet all key bytes at once automatically so you've to select which one to break.  
Run the  attack:

```bash
daredevil -c addr8_r_50_72072.config
```

Best key candidate should be 0x14 which is the first 6-bit key segment from the first round key.  
Indeed, as for a DES key 3032343234363236, the first round key is:

```
0 1 0 1 0 0 0 0 0 0 1 0 1 1 0 0 1 0 1 0 1 1 0 0 1 1 0 0 0 1 1 0 0 0 0 0 0 0 1 1 1 1 0 0 0 1 1 1
.........14 .........02 .........32 .........2C .........31 .........20 .........0F .........07
.............50 .............2C .............AC .............C6 .............03 .............C7
```
