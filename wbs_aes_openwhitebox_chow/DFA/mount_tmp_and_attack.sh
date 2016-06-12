#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_owb_chow.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/encryptECB tmp
cp ../target/key.txt tmp/key.txt.gold
cd tmp
./attack_owb_chow.py
