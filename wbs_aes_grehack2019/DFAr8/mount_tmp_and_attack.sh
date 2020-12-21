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
ulimit -c 0
# Compute reference output
./main.gold 74657374746573747465737474657374 > r8faults
# DFA, keep all major faults at Level 7 (maxleaf=128b => level 7=1b)
./attack_gh19ctf.py|grep "Lvl 007.*MajorFault"|cut -d' ' -f 8 >> r8faults
# Convert 8th round faults into 9th round faults and run the attack
./attack_r9.py
# Result is the last round key, still need to invert AES keyschedule with
# aes_keyschedule <r10_key> 10
