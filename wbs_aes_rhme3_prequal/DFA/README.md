Usage
-----

Faulting multiple times the binary requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_rhme3p1.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/whitebox tmp/whitebox.gold
cd tmp
./attack_rhme3p1.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

```attack_rhme3p1.py``` contains three sets of parameters.

The first one is very conservative and doesn't require any knowledge, but it's also pretty slow:
```
# lazy settings:
minleaf=1, minfaultspercol=200
```

Typically you would even try first with the default settings and go for these slower ones if the default ones fail, which is the case here.

Now a quick look at the sections shows that the tables are not in a data section but somehow embedded into the code section:

```bash
readelf -W -S ../target/whitebox|egrep "(Name|data|text)"
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [14] .text             PROGBITS        00000000004006e0 0006e0 063682 00  AX  0   0 16
  [16] .rodata           PROGBITS        0000000000463d70 063d70 000190 00   A  0   0  8
  [25] .data             PROGBITS        0000000000665068 065068 000010 00  WA  0   0  8
```

So faster settings will try to NOP some code (this goes a bit quicker by crashing less).

It's also pointless to try with large ```maxleaf``` while ```minleaf``` must be lowered to one single byte to be able to target operands of instructions.

When looking to the graph of a trace (see [traces](../traces)), we see a large portion of the code is used to write the tables, which will be used in their natural order, so trying to target the one but last round means attacking code around 0x445000 to 0x455000 (~ offsets of 0x45000 to 0x55000 in the binary file). The attack takes about 2m30.

```
# faster settings:
faults=[('nop', lambda x: 0x90)], maxleaf=32, minleaf=1, minleafnail=1, minfaultspercol=2, addresses=(0x45000,0x55000)
```

Now once we identified a sweet spot and broke a first whitebox, imagine we've to break a similar one with another key, we can hit directly the right bytes and the attack takes less than 2s:

```
# damn faster settings:
maxleaf=1, minleaf=1, minfaultspercol=2, addresses=(0x47bf0,0x47c00)
```

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
Last round key #N found:
4E44EACD3F54F5B54A4FB15E0710B974

aes_keyschedule 4E44EACD3F54F5B54A4FB15E0710B974 10
K00: 61316C5F7434623133355F525F6F5235
```
