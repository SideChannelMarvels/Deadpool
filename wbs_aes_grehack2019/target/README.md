Original description
--------------------

title: Military grade whitebox

by Nathan R.  
250 points

Hey, I just found out this piece of code on a server. It must have been used for super important message ciphering but it does not look that strong.  
I also recovered a plaintext and its cipher when dumping the RAM :
```
plaintext : 57 68 6f 20 49 73 20 52 69 6a 6e 64 61 65 6c 20
ciphertext : 4b 95 f3 2b d1 9c ca 0a 81 64 9c 13 05 5d c9 f2
```
Can you help me by recovering the key ?  
Hint:  
Looks like PIRET and QUISQUATER found a way to do it, will you ?

Compilation
-----------

To run the provided code, we add a small main.c

```bash
tar xf 1573854749.4296393_e6b19a0f2ddea947e2166ddf9c7fa852e93cd3f4.tar.gz
gcc -o main main.c
```

Usage
-----

```bash
./main 57686f2049732052696a6e6461656c20
4b95f32bd19cca0a81649c13055dc9f2
```

Solution
--------

```bash
echo 57686f2049732052696a6e6461656c20|xxd -r -p|openssl enc -e -aes-128-ecb -K $(echo -n "GH19{AES is FUN}"|xxd -p) -nopad|xxd -p
4b95f32bd19cca0a81649c13055dc9f2
```
