#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_ches2016.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/*.[ch] ../target/Makefile tmp
cd tmp
make
cp -a wb_challenge wb_challenge.gold
./attack_ches2016.py
