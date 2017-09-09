#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_lee_case1.py ../../deadpool_dfa*.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/WB_LEE_CASE1_Client tmp
cp ../target/Tab.bin tmp/Tab.bin.gold
cd tmp
./attack_lee_case1.py
