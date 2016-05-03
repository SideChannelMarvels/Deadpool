#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re

d={}
d["out[137]"] = "t[10][14]"
d["out[140]"] = "t[10][11]"
d["out[131]"] = "t[10][4]"
d["out[144]"] = "t[10][1]"
d["out[136]"] = "t[10][10]"
d["out[146]"] = "t[10][7]"
d["out[139]"] = "t[10][0]"
d["out[147]"] = "t[10][13]"
d["out[128]"] = "t[10][3]"
d["out[135]"] = "t[10][12]"
d["out[132]"] = "t[10][6]"
d["out[138]"] = "t[10][9]"
d["out[145]"] = "t[10][8]"
d["out[143]"] = "t[10][2]"
d["out[142]"] = "t[10][15]"
d["out[141]"] = "t[10][5]"
t={}
f = open('nosuchcon_2013_whitebox_debug.log', 'r')
for line in reversed(f.readlines()):
    pattern = '(.+) = (rT[0-9]\[[0-9]+\]\[[0-9]+\])\[(.*)\]'
    patternf = '(.+) = (fT\[[0-9]+\])\[(.*)\]'
    pattern_16 = '(.+) = (xT[0-2])\[(.+)\]\[(.+)\]'

    match = re.search(pattern, line)
    if match:
        dst = match.group(1)
        table = match.group(2)
        fetched = match.group(3)
#        print dst, table, fetched
        t[dst]=(table, fetched)

    match = re.search(patternf, line)
    if match:
        dst = match.group(1)
        table = match.group(2)
        fetched = match.group(3)
#        print dst, table, fetched
        t[dst]=(table, fetched)

    match = re.search(pattern_16, line)
    if match:
        dst = match.group(1)
        table = match.group(2)
        fetched1 = match.group(3)
        fetched2 = match.group(4)
#        print dst, table, fetched1, fetched2
        t[dst]=(table, fetched1, fetched2)

f = open('ssa_algorithm_clean', 'r')
for line in reversed(f.readlines()):
    pattern = '(.+) = (T[H]?_[A-F0-9]+)\[(.+?)\];'
    pattern_16 = '(.+) = (T16_[A-F0-9]+)\[(.+)\]\[(.+)\];'

    match = re.search(pattern, line)
    if match:
        dst = match.group(1)
        table = match.group(2)
        fetched = match.group(3)
#        print dst, table, fetched
        assert dst in d
#        print dst, "->", d[dst], "->", t[d[dst]]
        (d[table], d[fetched])=t[d[dst]]
        print d[table], "=", table
#        print "d", fetched, "=", d[fetched]

    match = re.search(pattern_16, line)
    if match:
        dst = match.group(1)
        table = match.group(2)
        fetched1 = match.group(3)
        fetched2 = match.group(4)
#        print dst, table, fetched1, fetched2
        assert dst in d
        (d[table], d[fetched1], d[fetched2])=t[d[dst]]
        print d[table], "=", table
#        print "d", fetched1, "=", d[fetched1]
#        print "d", fetched2, "=", d[fetched2]
