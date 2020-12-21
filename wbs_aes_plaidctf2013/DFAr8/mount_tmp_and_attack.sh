#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_plaidctf.py attack_r9.py ../../deadpool_dfa.py tmp
cp ../../../JeanGrey/phoenixAES/phoenixAES/__init__.py tmp/phoenixAES.py
cp ../target/drmless tmp
cd tmp
ulimit -c 0
echo "341AFA02F295E6FC0B089E41C630274B" > r8faults
./attack_plaidctf.py|grep MajorFault|cut -c 78-109 >> r8faults
./attack_r9.py
