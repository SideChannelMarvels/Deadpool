Usage
-----

As explained in wbs_aes_hacklu2009/traces/README.md, we'll use TracerGrind, Wine and xdotool to automate the attack on this Windows GUI crackme.

Remember first to start another Wine process from another terminal to avoid tracing the Wine server itself:

```bash
wine notepad.exe
```

Of course make sure not to interfere with keyboard or mouse during the attack.

Recording 20 traces:

```bash
./Tracer2bin.py
...
00000 D77EAE3D6557FDA6E4E013435B984F50 -> ?
...
00001 A7F1A463EDA8248E357CB14B2DBD74C9 -> ?
...
```

Converting traces for Daredevil:

```bash
./bin2daredevil.py
```

Executing the differential analysis on the converted traces, here on addresses:

```bash
daredevil -c addr8_r_20_5888.config
```

Or on stack writes:

```bash
daredevil -c stack8_w_20_3008.config
```

Both work equally well on 20 traces with correlations being equal to 1, which means no internal encoding was used in this white-box.

