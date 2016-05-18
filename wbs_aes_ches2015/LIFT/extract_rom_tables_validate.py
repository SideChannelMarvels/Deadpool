#!/usr/bin/env python

# pad=1..16 whatever
pad=1

def validate(t):
    stats=[0]*256
    for x in t:
        stats[ord(x)]+=1
    # all elements must appear the same nr of times
    assert stats.count(stats[0]) == len(stats)

def inv(t):
    tinv=[0]*256
    for i in range(len(t)):
        tinv[ord(t[i])]=chr(i)
    return ''.join(tinv)

rom = open('../target/rom.bin', 'rb').read()

with open('wbt_rom', 'wb') as wbt:
    init_invsub = rom[0x4008:0x4008+0x100]
    validate(init_invsub)
    init_sub = inv(init_invsub)
    validate(init_sub)
    wbt.write(init_sub)
    wbt.write(init_invsub)
    final_IV1 = rom[0x892a6+(0x100*pad):0x892a6+(0x100*pad)+0x100]
    final_sub = ''. join([final_IV1[i^pad] for i in range(256)])
    validate(final_sub)
    wbt.write(final_sub)
    final_invsub = inv(final_sub)
    validate(final_invsub)
    wbt.write(final_invsub)
    xorTable1=""
    for i in range(0x10000/0x4000):
        xorTable1+=rom[0x50002+(i*0x4000):0x50002+(i*0x4000)+0x3ff0]
        xorTable1+=rom[0x5208+(i*16):0x5208+(i*16)+16]
    validate(xorTable1)
    xorTable2=""
    for i in range(0x10000/0x4000):
        xorTable2+=rom[0x60002+(i*0x4000):0x60002+(i*0x4000)+0x3ff0]
        xorTable2+=rom[0x5248+(i*16):0x5248+(i*16)+16]
    validate(xorTable2)
    xorTable3=""
    for i in range(0x10000/0x4000):
        xorTable3+=rom[0x70002+(i*0x4000):0x70002+(i*0x4000)+0x3ff0]
        xorTable3+=rom[0x5288+(i*16):0x5288+(i*16)+16]
    validate(xorTable3)
    wbt.write(xorTable1)
    wbt.write(xorTable2)
    wbt.write(xorTable3)
    for i in range(9):
        roundTable=rom[0x8002+0x8000*i:0x8002+0x8000*i+0x2000]+rom[0x8002+0x8000*i+0x4000:0x8002+0x8000*i+0x4000+0x2000]
        validate(roundTable)
        wbt.write(roundTable)
    finalTable = rom[0x4208:0x4208+(0x100*16)]
    validate(finalTable)
    wbt.write(finalTable)
