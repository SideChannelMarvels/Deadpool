Compilation
-----------

```
cd src
make
mv ctf2 ..
```

Challenge description
---------------------

From https://github.com/LedgerHQ/CTF/blob/master/ctf2018/README.md

```
Challenge 2: Access control

You’re given a program which runs on a remote host.
This program implements an access control based on AES.
You’re asked to find the input which outputs “**** Login Successful ****”.
You’re not asked to modify the exec so that it outputs  “**** Login Successful ****” (it’s supposed to run on my machine).
It’s compiled on Linux-64, and is not striped.
```

Solution
--------

```bash
$ echo 13376942133769421337694213376942|xxd -r -p|openssl enc -d -aes-128-ecb -nopad -K f0331ce0266adace86a8a13bfa146740|xxd -p
0ab7982c0ec65fc9c2412d527470d768
$ ./ctf2 0ab7982c0ec65fc9c2412d527470d768
**** Login Successful ****
```
