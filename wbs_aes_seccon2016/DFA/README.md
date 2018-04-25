Usage
-----

Faulting multiple times the binary requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=50m tmpfs tmp
cp attack_seccon2016.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/main ../target/libnative-lib.so tmp
cp ../target/libnative-lib.so tmp/libnative-lib.so.gold
cd tmp
./attack_seccon2016.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
aes_keyschedule 040D08DA68001026F3DC0D68897148B4 10
K00: 6C2893F21B6185E8567238CB78184945
```
