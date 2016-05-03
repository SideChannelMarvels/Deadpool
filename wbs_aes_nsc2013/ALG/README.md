Algebraic attack
----------------

We'll perform an algebraic attack on one round (any between the 2nd and the 9th).
A description of the attack is available at http://wiki.yobi.be/wiki/NSC_Writeups#Oppida.2FNoSuchCon_challenge

This attack require the tables extracted from the reverse-engineering step, so if you didn't do it yet:

```bash
cd ../RE
./extract.sh
cd -
```

Then:

```bash
gcc -o nosuchcon_2013_whitebox_generator_noenc_nokey nosuchcon_2013_whitebox_generator_noenc_nokey.c
./nosuchcon_2013_whitebox_generator_noenc_nokey
gcc -o nosuchcon_2013_whitebox_nsc_break nosuchcon_2013_whitebox_nsc_break.c
./nosuchcon_2013_whitebox_nsc_break
```

It breaks one round key and reverts the AES key scheduling till

```K00: 4E5343234F707069646123B8DCE442D0```

We can see that the challenge key starts actually with:

```bash
echo -n "NSC#Oppida#"|xxd -p
4e5343234f707069646123
```
