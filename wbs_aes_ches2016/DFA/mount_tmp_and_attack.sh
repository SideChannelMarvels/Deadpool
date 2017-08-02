#!/bin/bash


# Compile the challenge from scratch or use the precompiled binary?
# Problems may arise when you compile it yourself because addresses may have changed
# and you will need to tune attack_ches2016.py "addresses=[0x3b000,0x3f000]" accordingly
# (or just remove that parameter and go for a slower attack)
USE_PRECOMPILED_CHALLENGE=true

mkdir -p tmp
mount|grep -q $(pwd)/tmp || sudo mount -t tmpfs -o mode=01777,size=20m tmpfs tmp
cp attack_ches2016.py ../../deadpool_dfa.py ../../../JeanGrey/phoenixAES.py tmp
$USE_PRECOMPILED_CHALLENGE || cp ../target/*.[ch] ../target/Makefile tmp
$USE_PRECOMPILED_CHALLENGE && cp ../target/wb_challenge tmp
cd tmp
$USE_PRECOMPILED_CHALLENGE || make
cp -a wb_challenge wb_challenge.gold
./attack_ches2016.py
