#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=50m tmpfs tmp
cp attack_seccon2016.py ../../deadpool_dfa.py tmp
cp ../../../JeanGrey/phoenixAES/phoenixAES/__init__.py tmp/phoenixAES.py
cp ../target/main64 ../target/libnative-lib.so tmp
cp ../target/libnative-lib.so tmp/libnative-lib.so.gold
cd tmp
ulimit -c 0
./attack_seccon2016.py
