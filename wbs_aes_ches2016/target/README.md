Compilation
-----------

The original challenge contains a statically linked binary as well as the source code.

```bash
tar xf ches_wb_challenge.tgz
```

If one wants to recompile the binary:

```bash
make
```

Usage
-----

```bash
./wb_challenge 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
INPUT:     00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 
OUTPUT:    c1 bd 88 bf e6 5e 87 01 3f 3f 41 96 c1 8a f3 68 
```

Solution
--------

```bash
echo c1bd88bfe65e87013f3f4196c18af368|xxd -r -p|openssl enc -d -aes-128-ecb -K DEC1A551F1EDDEC0DE4B1DAE5C0DE511 -nopad|xxd -p
000102030405060708090a0b0c0d0e0f
```
