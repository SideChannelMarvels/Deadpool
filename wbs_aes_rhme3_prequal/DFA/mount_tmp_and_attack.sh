#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_rhme3p1.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
cp ../target/whitebox tmp/whitebox.gold
cd tmp
./attack_rhme3p1.py
