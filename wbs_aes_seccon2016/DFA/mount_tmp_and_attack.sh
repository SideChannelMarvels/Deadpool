#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=50m tmpfs tmp
cp attack_seccon2016.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/main64 ../target/libnative-lib.so tmp
cp ../target/libnative-lib.so tmp/libnative-lib.so.gold
cd tmp
./attack_seccon2016.py
