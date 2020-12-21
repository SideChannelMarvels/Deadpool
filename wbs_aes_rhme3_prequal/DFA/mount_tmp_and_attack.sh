#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_rhme3p1.py ../../deadpool_dfa.py tmp
cp ../../../JeanGrey/phoenixAES/phoenixAES/__init__.py tmp/phoenixAES.py
cp ../target/whitebox tmp/whitebox.gold
cd tmp
ulimit -c 0
./attack_rhme3p1.py
