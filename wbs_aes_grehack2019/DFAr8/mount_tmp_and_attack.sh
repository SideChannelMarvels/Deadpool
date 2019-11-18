#!/bin/bash

(
  cd ../target
  tar xf 1573854749.4296393_e6b19a0f2ddea947e2166ddf9c7fa852e93cd3f4.tar.gz
  gcc -o main main.c
)
mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_gh19ctf.py attack_r9.py ../../deadpool_dfa.py tmp
cp ../../../JeanGrey/phoenixAES/phoenixAES/__init__.py tmp/phoenixAES.py
cp ../target/main tmp/main.gold
cd tmp
./main.gold 74657374746573747465737474657374 > r8faults
./attack_gh19ctf.py|grep MajorFault|cut -d' ' -f 8 >> r8faults
./attack_r9.py
