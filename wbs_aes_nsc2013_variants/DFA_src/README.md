Usage
-----

When you have the source code of a white-box, either by design (the key lies in the tables anyway) or by reverse-engineering, you can easily inject faults.

The fault must affect one single byte of the AES state between the last two MixColumn operations, i.e. after MixColumn of the 8th round and before MixColumn of the 9th round.

When you don't know exactly where to inject the faults, the strategy is simple:

 * Try to fault a byte at a given position
   * If all the output changes, the fault is injected too early
   * If only one byte of theoutput change, the fault is injected too late
   * If four bytes are affected, you're good!
 * Then inject various faults in bytes of the four columns
   * In principle 2 faults per column is enough, more doesn't hurt

So, first we generate a white-box:

```bash
gcc -o nosuchcon_2013_whitebox_allenc_generator nosuchcon_2013_whitebox_allenc_generator.c
gcc -o nosuchcon_2013_whitebox_allenc nosuchcon_2013_whitebox_allenc.c
./nosuchcon_2013_whitebox_allenc_generator
```

Then we attack it:

```bash
gcc -o nosuchcon_2013_whitebox_allenc_DFA nosuchcon_2013_whitebox_allenc_DFA.c
python3 DFA.py
```

Given the structure of this white-box we can even inject a fault in the 8th MixColumn, before the xors, which is done in the version DFA2.

```bash
gcc -o nosuchcon_2013_whitebox_allenc_DFA2 nosuchcon_2013_whitebox_allenc_DFA2.c
python3 DFA2.py
```

Adapt it to your setup if needed. It requires ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
aes_keyschedule FE6DA2ABE15DEDF1A40A82B0608D31CD 10
K00: 4B45595F4B45595F4B45595F4B45595F
```
