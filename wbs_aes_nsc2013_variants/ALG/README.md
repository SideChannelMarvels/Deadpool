Algebraic attack
----------------

This is exactly the same attack as for the [original wbs_aes_nsc2013](../../wbs_aes_nsc2013/ALG/README.md).

We'll perform an algebraic attack on one round (any between the 2nd and the 9th).
A description of the attack is available at https://wiki.yobi.be/index.php/NSC_Writeups#Oppida.2FNoSuchCon_challenge

First we generate a white-box:

```bash
gcc -o nosuchcon_2013_whitebox_allenc_generator ../target/nosuchcon_2013_whitebox_allenc_generator.c
gcc -o nosuchcon_2013_whitebox_allenc ../target/nosuchcon_2013_whitebox_allenc.c
./nosuchcon_2013_whitebox_allenc_generator
```

Then we attack it, with an "empty" white-box as reference:

```bash
gcc -o nosuchcon_2013_whitebox_generator_noenc_nokey nosuchcon_2013_whitebox_generator_noenc_nokey.c
./nosuchcon_2013_whitebox_generator_noenc_nokey
gcc -o nosuchcon_2013_whitebox_nsc_break nosuchcon_2013_whitebox_nsc_break.c
./nosuchcon_2013_whitebox_nsc_break
```

It breaks one round key and reverts the AES key scheduling till

```K00: 4B45595F4B45595F4B45595F4B45595F```
