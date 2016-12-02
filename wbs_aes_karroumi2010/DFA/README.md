Usage
-----

Faulting multiple times Karroumi tables requires many disk writes so we'll do it in a tmpfs.  
All steps are performed by ```mount_tmp_and_attack.sh```:

```bash
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_karroumi.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py ../target/main tmp
cd tmp
./main --create-table tables_karroumi_extenc.tbl.gold --extEnc=1
./attack_karroumi.py
```

Adapt it to your setup if needed. It requires ```deadpool_dfa.py``` from this repository and ```phoenixAES.py``` from [JeanGrey repository](https://github.com/SideChannelMarvels/JeanGrey).

A word about ```deadpool_dfa.Acquisition``` settings:
  * ```maxleaf=64, minleaf=4, minleafnail=1```: table is Boost serialized data so we can't fault very large areas at once.
  * ```addresses=(0x57000, 0x5A000)```: it's not required, only provided here for a faster result but in a real case if you don't guess where to shoot, just try everywhere.

Result is the last round key, so to roll back key scheduling up to initial AES key, one can e.g. use ```aes_keyschedule``` from Stark project.

```bash
aes_keyschedule D014F9A8C9EE2589E13F0CC8B6630CA6 10
K00: 2B7E151628AED2A6ABF7158809CF4F3C
```
