Usage
-----

```bash
./WB_LEE_CASE1_Client $(echo -n testtesttesttest|xxd -p|sed 's/\(..\)/\1 /g')
INPUT : 74 65 73 74 74 65 73 74 74 65 73 74 74 65 73 74 
OUTPUT : ad ae 6a 69 95 3f 6f dc f8 fa 44 bb ea b6 6b 10 

./WB_LEE_CASE1_Client 74 65 73 74 74 65 73 74 74 65 73 74 74 65 73 74
INPUT : 74 65 73 74 74 65 73 74 74 65 73 74 74 65 73 74 
OUTPUT : ad ae 6a 69 95 3f 6f dc f8 fa 44 bb ea b6 6b 10 

```

Solution
--------

```bash
echo -n testtesttesttest|openssl enc -e -aes-128-ecb -nopad -K 00112233445566778899AABBCCDDEEFF|xxd -p
adae6a69953f6fdcf8fa44bbeab66b10
```
