#!/bin/bash

gcc -o nosuchcon_2013_whitebox_debug nosuchcon_2013_whitebox_debug.c
./nosuchcon_2013_whitebox_debug > nosuchcon_2013_whitebox_debug.log
python ssa_algorithm_step1.py > ssa_algorithm_clean
python ssa_algorithm_step2.py|sort|uniq > tables
python ssa_algorithm_step3.py > gentable.c
gcc -o gentable gentable.c
./gentable
cd result
gcc -o nosuchcon_2013_whitebox nosuchcon_2013_whitebox.c
