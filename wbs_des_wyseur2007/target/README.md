Usage
-----

```bash
./wbDES 11 22 33 44 55 66 77 88

INPUT:    11 22 33 44 55 66 77 88.
OUTPUT:   c4 03 d3 2e 2b c6 cf ee.
```

Solution
--------

```bash
echo 11 22 33 44 55 66 77 88|xxd -r -p|openssl enc -e -des-ecb -nopad -K 3032343234363236|xxd -p
c403d32e2bc6cfee
```
