#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

bin2daredevil(keyword='bits', config={'algorithm':'DES', 'position':'LUT/DES_SBOX', 'correct_key':'0xfd4185ff66a94afd'})
