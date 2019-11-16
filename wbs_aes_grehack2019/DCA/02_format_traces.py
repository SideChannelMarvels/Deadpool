#!/usr/bin/env python

import sys
sys.path.insert(0, '../../')
from deadpool_dca import *

bin2daredevil(keyword='bits', config={'algorithm':'AES', 'position':'LUT/AES_AFTER_SBOX'})
