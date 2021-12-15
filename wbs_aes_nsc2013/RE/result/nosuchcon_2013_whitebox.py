#!/usr/bin/env python3
import sys

t=[[None]*16 for _ in range(12)]
t[0]=list(bytes.fromhex(sys.argv[1]))

roundTables=[[[None]*4 for _ in range(16)] for _ in range(9)]
finalTable=[None]*16
with open("wbt_nsc", "rb") as f:
    initSub_sub = list(f.read(0x100))
    initSub_inv_sub = list(f.read(0x100))
    finalSub_sub = list(f.read(0x100))
    finalSub_inv_sub = list(f.read(0x100))
    xorTables0 = list(f.read(0x10000))
    xorTables1 = list(f.read(0x10000))
    xorTables2 = list(f.read(0x10000))
    for i in range(9):
        for j in range(16):
            for k in range(4):
                roundTables[i][j][k] = list(f.read(0x100))
    for i in range(16):
        finalTable[i] = list(f.read(0x100))

print("Enc in:   ", end='')
print(" ".join("%02X" % x for x in t[0]))

for round in range(1,10):
    for i in range(16):
        b = [0, 0, 0, 0]
        for j in range(4):
            b[j] = roundTables[round-1][i][j][t[round-1][j*4+((i+j)%4)]];
            t[round][i] = xorTables2[(xorTables0[(b[0]<<8)|b[1]] << 8) | xorTables1[(b[2]<<8)|b[3]]]

for i in range(16):
    t[10][i//4 + (i%4)*4] = finalTable[i][t[9][(i&(~3)) +((i+i//4)%4)]]

print("Enc out:  ", end='');
print(" ".join("%02X" % x for x in t[10]))