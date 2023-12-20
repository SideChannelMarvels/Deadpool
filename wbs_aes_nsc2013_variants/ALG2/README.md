Second algebraic attack
-----------------------

Compared to the original NoSuchCon 2013 challenge, here we have the external encodings tables therefore there is another attack possible on the finalTable with the knowledge of the output external encoding table.

A description of the attack is available at https://wiki.yobi.be/index.php/CHES2015_Writeup#Second_step_revisited.2C_the_sequel

First we generate a white-box:

```bash
gcc -o nosuchcon_2013_whitebox_allenc_generator ../target/nosuchcon_2013_whitebox_allenc_generator.c
gcc -o nosuchcon_2013_whitebox_allenc ../target/nosuchcon_2013_whitebox_allenc.c
./nosuchcon_2013_whitebox_allenc_generator
```

Then we attack it:

```bash
python nosuchcon_2013_whitebox_finaltable_break.py
```

It breaks the last round key and reverts the AES key scheduling:

```kR10= FE6DA2ABE15DEDF1A40A82B0608D31CD => K= 4B45595F4B45595F4B45595F4B45595F```
