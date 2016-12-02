Usage
-----

Faulting multiple times the tables requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_nsc.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/nosuchcon_2013_whitebox_allenc.c ../target/nosuchcon_2013_whitebox_allenc_generator.c tmp
cd tmp
gcc -o nosuchcon_2013_whitebox_allenc_generator nosuchcon_2013_whitebox_allenc_generator.c
gcc -o nosuchcon_2013_whitebox_allenc nosuchcon_2013_whitebox_allenc.c
./nosuchcon_2013_whitebox_allenc_generator
mv wbt_allenc wbt_allenc.gold
./attack_nsc.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
aes_keyschedule FE6DA2ABE15DEDF1A40A82B0608D31CD 10
K00: 4B45595F4B45595F4B45595F4B45595F
```
