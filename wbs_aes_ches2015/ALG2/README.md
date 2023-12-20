Second algebraic attack
-----------------------

We have the external encodings tables therefore there is another attack possible on the finalTable with the knowledge of the output external encoding table.

A description of the attack is available at https://wiki.yobi.be/index.php/CHES2015_Writeup#Second_step_revisited.2C_the_sequel

```bash
python ches_2015_whitebox_finaltable_break.py
```

It breaks the last round key and reverts the AES key scheduling:

```kR10= 66CA8706495D4543EAD87A31199E5889 => K= 4A7BB85DC1446AA1112222223B2A99FF```
