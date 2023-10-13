cf https://github.com/LedgerHQ/CTF/blob/master/ctf2018/CTF2/ctf2_GeorgesGanerot

From the author:
*With a little reverse engineering, we noticed that the implementation was doing 15 AES computations in random order. (40 steps required to do a full AES computation), masking is used in different part of the algorithm.*  
*The state 7 contains the state of the good AES.*  
*We focused on state7, after the initial SBOX step (that also include the AddRoundKey).*  
*The gdb_commands script forces the masks to be 0, and ensure that we only work on the 7th state in the schedule call. It exists when the first sbox operation is done.*  
*The python script generates 3 values (00,01,02) for a specific byte, and retrieve the key that would yield such values in the output state.*

```
$ ./run.py
checking key for 8c a1 89
Found key: f0
checking key for c3 23 c7
Found key: 33
checking key for 9c a4 72
Found key: 1c
checking key for e1 f8 98
Found key: e0
checking key for f7 cc 36
Found key: 26
checking key for 2 7f 45
Found key: 6a
checking key for 57 b9 61
Found key: da
checking key for 8b 8a 4b
Found key: ce
checking key for 44 17 5f
Found key: 86
checking key for c2 d3 ac
Found key: a8
checking key for 32 e0 a
Found key: a1
checking key for e2 80 12
Found key: 3b
checking key for 2d f 41
Found key: fa
checking key for fa 59 47
Found key: 14
checking key for 85 33 4d
Found key: 67
checking key for 9 83 2c
Found key: 40
Key:    f0331ce0266adace86a8a13bfa146740
Cipher: 13376942133769421337694213376942
Plain:  0ab7982c0ec65fc9c2412d527470d768
```
