Usage
-----

Using Tracer to acquire execution traces, here recording the lowest byte of each address from which data is being read:

```bash
./trace_it.py
00000 F9B247990D90C1F3 -> AE90B8FBD1FC36CD
00001 0FD6AE2BD86D6178 -> EBA637804BB3417F
00002 222C10E967C93DFB -> 3130E6D763CB113D
00003 582CBC2F0DBAA3DA -> 36F1FDF96EF99F5C
00004 77F7E7B85708F642 -> 5526806DBBF8F57C
00005 3FAD5C7299310488 -> 0162523640E445B9
00006 04305FFFA97E76FF -> E4CFAD2453A2B74E
```

By default the script is using TracerPIN but if you want you can change it to use TracerGrind, cf commented line

Executing the differential analysis on the converted traces:

```bash
daredevil -c mem_addr1_rw1_100_72080.config
```

Here we already provided the correct DES key in the configuration and the tool returns the rank of the correct 6-bit round key segments amongst all the key candidates.
Typically 20 traces are enough to rank the correct ones in first position.

To demonstrate an attack without knowing the key, edit mem_addr1_rw1_100_72080.config:

* Comment out the correct_key
* Choose which key segment to break, e.g.: bytenum=0

At this moment Daredevil doesn't attack yet all key bytes at once automatically so you've to select which one to break.  
Run the  attack:

```bash
daredevil -c mem_addr1_rw1_100_72080.config
```

Best key candidate should be 0x14 which is the first 6-bit key segment from the first round key.  
Indeed, as for a DES key `3032343234363236`, the first round key is:

```
0 1 0 1 0 0 0 0 0 0 1 0 1 1 0 0 1 0 1 0 1 1 0 0 1 1 0 0 0 1 1 0 0 0 0 0 0 0 1 1 1 1 0 0 0 1 1 1
.........14 .........02 .........32 .........2C .........31 .........20 .........0F .........07
.............50 .............2C .............AC .............C6 .............03 .............C7
```

One can find more details on the project wiki at https://github.com/SideChannelMarvels/Deadpool/wiki/Tutorial-%231:-DCA-against-Wyseur-2007-challenge
