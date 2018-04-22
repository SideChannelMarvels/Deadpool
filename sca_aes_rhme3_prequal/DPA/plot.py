#!/usr/bin/env python3

import sys
import matplotlib.pyplot as plt
import numpy as np

ntraces=300
nsamples=6095
alltraces=[]
with open(sys.argv[1], 'rb') as trs:
    for i in range(ntraces):
        alltraces.append(np.fromfile(trs, dtype=np.float32, count=nsamples))

for i in range(1):
    plt.plot(alltraces[i])
plt.show()
