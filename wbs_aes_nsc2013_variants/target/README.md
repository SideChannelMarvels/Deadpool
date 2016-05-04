Usage
-----

Here are three versions based on NoSuchCon 2013 whitebox generator.

```nosuchcon_2013_whitebox_pastebin_generator.c``` is the original generator code from Eloi Vanderbéken, meant to be compiled under Windows.
We won't use it.

All encodings version
---------------------

```nosuchcon_2013_whitebox_allenc_generator.c``` is the generator code adapted for Linux.  
```nosuchcon_2013_whitebox_allenc.c``` is using the generated tables to encrypt any plaintext.  
This version uses internal encodings and external encodings, directly compensated around the white-box so in total it's a real AES encryption.

```bash
gcc -o nosuchcon_2013_whitebox_allenc_generator nosuchcon_2013_whitebox_allenc_generator.c
gcc -o nosuchcon_2013_whitebox_allenc nosuchcon_2013_whitebox_allenc.c
./nosuchcon_2013_whitebox_allenc_generator
./nosuchcon_2013_whitebox_allenc $(echo testtesttesttest|xxd -p)
Input:    74 65 73 74 74 65 73 74 74 65 73 74 74 65 73 74 
Enc in:   B2 B2 B2 B2 68 68 68 68 51 51 51 51 B2 B2 B2 B2 
Enc out:  57 65 7A 27 70 2C 07 05 DA 7D 12 B2 0C 63 0F CE 
Output:   4C 74 7B 5C 53 54 DB 2B 6D 13 39 C9 31 81 33 40 
```

Only internal encodings version
-------------------------------

```nosuchcon_2013_whitebox_noextenc_generator.c``` is the generator code but without external encodings.  
```nosuchcon_2013_whitebox_noextenc.c``` is using the generated tables to encrypt any plaintext.  
This version uses only internal encodings.

```bash
gcc -o nosuchcon_2013_whitebox_noextenc_generator nosuchcon_2013_whitebox_noextenc_generator.c
gcc -o nosuchcon_2013_whitebox_noextenc nosuchcon_2013_whitebox_noextenc.c
./nosuchcon_2013_whitebox_noextenc_generator
./nosuchcon_2013_whitebox_noextenc $(echo testtesttesttest|xxd -p)
Input:    74 65 73 74 74 65 73 74 74 65 73 74 74 65 73 74 
Enc in:   74 74 74 74 65 65 65 65 73 73 73 73 74 74 74 74 
Enc out:  4C 74 7B 5C 53 54 DB 2B 6D 13 39 C9 31 81 33 40 
Output:   4C 74 7B 5C 53 54 DB 2B 6D 13 39 C9 31 81 33 40 
```

No encodings
------------

```nosuchcon_2013_whitebox_noenc_generator.c``` is the generator code but without external encodings.  
```nosuchcon_2013_whitebox_noenc.c``` is using the generated tables to encrypt any plaintext.  
This version uses only internal encodings.

```bash
gcc -o nosuchcon_2013_whitebox_noenc_generator nosuchcon_2013_whitebox_noenc_generator.c
gcc -o nosuchcon_2013_whitebox_noenc nosuchcon_2013_whitebox_noenc.c
./nosuchcon_2013_whitebox_noenc_generator
./nosuchcon_2013_whitebox_noenc $(echo testtesttesttest|xxd -p)
Input:    74 65 73 74 74 65 73 74 74 65 73 74 74 65 73 74 
Enc in:   74 74 74 74 65 65 65 65 73 73 73 73 74 74 74 74 
Enc out:  4C 74 7B 5C 53 54 DB 2B 6D 13 39 C9 31 81 33 40 
Output:   4C 74 7B 5C 53 54 DB 2B 6D 13 39 C9 31 81 33 40 
```

Solution
--------

```bash
echo -n testtesttesttest|openssl enc -e -aes-128-ecb -nopad -K $(echo -n KEY_KEY_KEY_KEY_|xxd -p)|xxd -p
4c747b5c5354db2b6d1339c931813340
```
