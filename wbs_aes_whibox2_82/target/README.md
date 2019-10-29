Compilation
-----------

The challenge is a c-file with a function definition for AES_128_encrypt. Here is also provided a simple main file to call the AES_128_encrypt function. It can be compiled using make:

```bash
make main
```

Usage
-----

```bash
./main 00000000000000000000000000000000
9caace4a49e36f8247dc1d9fc93315c4
```

Solution
--------

```bash
echo 00000000000000000000000000000000|xxd -r -p|openssl enc -e -aes-128-ecb -nopad -K 676F2064616E63696E67206C696E6B73|xxd -p
9caace4a49e36f8247dc1d9fc93315c4
```
