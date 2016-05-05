#!/bin/bash

gcc -o nosuchcon_2013_whitebox_allenc_generator ../target/nosuchcon_2013_whitebox_allenc_generator.c
gcc -o nosuchcon_2013_whitebox_allenc ../target/nosuchcon_2013_whitebox_allenc.c
./nosuchcon_2013_whitebox_allenc_generator

gcc -o nosuchcon_2013_whitebox_allenc_DFA nosuchcon_2013_whitebox_allenc_DFA.c
python3 DFA.py

gcc -o nosuchcon_2013_whitebox_allenc_DFA2 nosuchcon_2013_whitebox_allenc_DFA2.c
python3 DFA2.py
