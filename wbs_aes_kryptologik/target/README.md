Compilation
-----------

The original challenge is written in JavaScript.
A port has been made in C and the large table externalized to a file.

To recompile it:

```bash
gcc -o DemoKey_table_encrypt DemoKey_table_encrypt.c
```

Usage
-----

```bash
./DemoKey_table_encrypt 59 6f 75 20 67 6f 74 20 62 72 6f 6b 65 6e 21 21
A482EDE858008238F3D300D2AEDC73E1
```

Solution
--------

Actually the challenge seems to not be exactly an AES.
It has 14 rounds as in AES-256 but the round keys don't follow the AES keyschedule.

In `solution` you can find `kryptologik_demo.py` that reproduces the client-server interactions of the website and
encrypt/decrypt messages using a modified AES-256 directly fed with the expanded key (the round keys).

The round keys are:
```
0D9BE960C438FF85F656BD48B78A0EE2
5A769B843C089BB1357FF628AE897D46
46359EFF9EFE9E1B41F1AA45CC250A12
AFBF8EFAD8F4878B8CF5E498342885FD
FF2C650283CF6605B42FB77E624B798D
9002F334E6C6288647B873A6145A15C5
DB33A8697D32BAA858D9EBB8FD266B3C
772DA79020B322D37C9B86DE70F8C3AA
600C5335D45052EB3BC418EAD9C611F3
3EC60262C17DE8D51F7E92E90715B965
4989783DD27A96D30C13C1ED687838F1
42ECC85F8C38152BE2D3F70154F12C8F
623247F2EA244F65A691A317F74934F2
E3A0F7B76729FB15ABE864512923F937
4A41F291E18FAEAFEA926076DDD3586B
```

So it's a kind of AES-1920 ;)
