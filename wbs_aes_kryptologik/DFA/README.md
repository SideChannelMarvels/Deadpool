Usage
-----

Faulting multiple times the tables requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_kryptologik.py ../../deadpool_dfa*.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/DemoKey_table_encrypt.c tmp
cp ../target/DemoKey_table.bin tmp/DemoKey_table.bin.gold
cd tmp
gcc -o DemoKey_table_encrypt DemoKey_table_encrypt.c
./attack_kryptologik.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` and
```deadpool_dfa_experimental.py``` from this repository and ```phoenixAES.py```
from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

`attack_kryptologik.py` is using an experimental attack to retrieve all the round keys
with DFA attacks because revealing the last round key and reverting it to an AES-128 key
failed, then revealing the last two round keys and reverting them to an AES-192 or
an AES-256 prove both to be wrong too.
