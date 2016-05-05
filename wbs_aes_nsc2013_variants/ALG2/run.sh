#!/bin/bash

gcc -o nosuchcon_2013_whitebox_allenc_generator ../target/nosuchcon_2013_whitebox_allenc_generator.c
gcc -o nosuchcon_2013_whitebox_allenc ../target/nosuchcon_2013_whitebox_allenc.c
./nosuchcon_2013_whitebox_allenc_generator
python nosuchcon_2013_whitebox_finaltable_break.py
