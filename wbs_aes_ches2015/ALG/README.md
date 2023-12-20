Algebraic attack
----------------

This is exactly the same attack as for the [original wbs_aes_nsc2013](../../wbs_aes_nsc2013/ALG/README.md).

We'll perform an algebraic attack on one round (any between the 2nd and the 9th).
A description of the attack is available at https://wiki.yobi.be/index.php/NSC_Writeups#Oppida.2FNoSuchCon_challenge

Once the tables are extracted from ```rom.bin``` (cf [../LIFT](../LIFT/README.md)),
we attack it, with an "empty" white-box as reference:

```bash
gcc -o ches_2015_whitebox_generator_noenc_nokey ches_2015_whitebox_generator_noenc_nokey.c
./ches_2015_whitebox_generator_noenc_nokey
gcc -o ches_2015_whitebox_rom_break ches_2015_whitebox_rom_break.c
./ches_2015_whitebox_rom_break
```

It breaks one round key and reverts the AES key scheduling till

```K00: 4A7BB85DC1446AA1112222223B2A99FF```
