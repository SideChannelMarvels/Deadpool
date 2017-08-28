Description
-----------

The original challenge contains an ELF 64-bit binary with the following description:

```
Qualifiers - 1pts
White Box Unboxing

Here is a binary implementing a cryptographic algorithm.
You provide an input and it produces the corresponding output.
Can you extract the key?
```


Usage
-----

Pass the plaintext as raw via stdin:

```bash
echo 000102030405060708090a0b0c0d0e0f|xxd -r -p|./whitebox --stdin
4f 0c 7f 43 85 f4 cf 72 8b 8c a7 81 60 ad be 67
```

It's also possible to pass it as raw in argument but you'll get troubles if you try to pass some null bytes...

Solution
--------

```bash
echo 4f0c7f4385f4cf728b8ca78160adbe67|xxd -r -p|openssl enc -d -aes-128-ecb -K 61316C5F7434623133355F525F6F5235 -nopad|xxd -p
000102030405060708090a0b0c0d0e0f
echo 61316C5F7434623133355F525F6F5235|xxd -r -p
a1l_t4b135_R_oR5
```
