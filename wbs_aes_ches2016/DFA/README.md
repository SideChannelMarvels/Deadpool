Usage
-----

Faulting multiple times WB tables requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_ches2016.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/*.[ch] ../target/Makefile tmp
cd tmp
make
cp -a wb_challenge wb_challenge.gold
./attack_ches2016.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

A word about ```deadpool_dfa.Acquisition``` settings:
  * ```minfaultspercol=200```: a first test shows no result because of false positives so here we just increase the number of good fauls we want before trying to break the equations.
  * ```addresses=(0x21aa0, 0xa0ac0)```: it's not required, only provided here for a faster result.
Address range is the range of the ```.data``` section, easily deduced from:

```bash
objdump -h wb_challenge
Sections:
[...]
Idx Name          Size      VMA               LMA               File off  Algn
 24 .data         0007f020  0000000000621aa0  0000000000621aa0  00021aa0  2**5
```

Another option would be to inject faults directly in the source code as it's available, but it's kind of flattened and takes time to get compiled so all in all that approach would require more time to find the sweet spot where to inject faults.

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
aes_keyschedule 9FC592CC67D77BF02F42748C3E171995 10
K00: DEC1A551F1EDDEC0DE4B1DAE5C0DE511
```
