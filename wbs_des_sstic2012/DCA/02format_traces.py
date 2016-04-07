#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

T=TracerPIN(filters=[FILTER('bits')], blocksize=8)
T.bin2daredevil(config={'algorithm':'DES', 'position':'LUT/DES_SBOX', 'correct_key':'0xfd4185ff66a94afd'})
