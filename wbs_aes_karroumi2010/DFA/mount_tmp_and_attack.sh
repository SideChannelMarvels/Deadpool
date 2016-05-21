#!/bin/bash

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_karroumi.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py ../target/main tmp
cd tmp
./main --create-table tables_karroumi_extenc.tbl.gold --extEnc=1
./attack_karroumi.py
