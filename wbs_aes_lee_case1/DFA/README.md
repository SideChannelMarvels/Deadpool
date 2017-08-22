Usage
-----

Faulting multiple times Lee CASE1 tables requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_lee_case1.py ../../deadpool_dfa*.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/WB_LEE_CASE1_Client ../target/Tab.bin tmp
cp ../target/WB_LEE_CASE1_Client tmp/WB_LEE_CASE1_Client.gold
cp ../target/Tab.bin tmp/Tab.bin.gold
cd tmp
./attack_lee_case1.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

A word about ```deadpool_dfa.Acquisition``` settings:
  * ```addresses=(0xC0000, 0xC3000)```: it's not required, only provided here for a faster result but in a real case if you don't guess where to shoot, just try everywhere. In that case, some false positives will appear in 0x40000-0x48000 and you've to set ```minfaultspercol``` to a higher value.

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
aes_keyschedule 36D024461D84B8375FC0F9C04CBAB6BB 10
K00: 00112233445566778899AABBCCDDEEFF
```
