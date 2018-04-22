Usage
-----

You need first to generate a white-box if not yet done:

```bash
cd ../target
gcc -o nosuchcon_2013_whitebox_noenc_generator nosuchcon_2013_whitebox_noenc_generator.c
gcc -o nosuchcon_2013_whitebox_noenc nosuchcon_2013_whitebox_noenc.c
./nosuchcon_2013_whitebox_noenc_generator
```

Using Tracer to acquire execution traces, here recording the lowest byte of each address from which data is being read:

```bash
./trace_it.py
00000 EAC8551CE4F966AB5DFDB9F46FCCEAF6 -> 67A26DB7A35C03F6CB458CC4C0206848
00001 4B03B32F200F8AE4C94938C7ED1EF53D -> 0040B486E9B6357845B858EE509D4774
00002 12A5E378132A0FAA63ED7EAB54705A19 -> 2336C1246B206DD8BB078C5B589DA349
00003 99FC7CDFBB91D2D22BF4C9BE6E61A11F -> E1916A81707453C268345F32247F7690
00004 B1D782D4CA74B6B50A01665AE0E75C51 -> B495554EDC9D7C4C0C4D4F4CC148365F
...
```

By default the script is using TracerGrind but if you want you can change it to use TracerPIN

Executing the differential analysis on the converted traces:

```bash
daredevil -c mem_addr1_rw1_2000_17096.attack_sbox.config
daredevil -c mem_addr1_rw1_2000_17096.attack_multinv.config
```

Actually the 8-bit wide encoding used in those challenges prevent using the DCA attack so we can only apply successfully DCA on the version without internal encodings.
See DFA for attacks against the version with all encodings.
