Compilation
-----------

```bash
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
