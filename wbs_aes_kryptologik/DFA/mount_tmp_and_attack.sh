#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_kryptologik.py ../../deadpool_dfa*.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/DemoKey_table_encrypt.c tmp
cp ../target/DemoKey_table.bin tmp/DemoKey_table.bin.gold
cd tmp
gcc -o DemoKey_table_encrypt DemoKey_table_encrypt.c
./attack_kryptologik.py
