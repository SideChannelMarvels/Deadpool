#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re

code = open('0vercl0k_writeup/wbaes128_solve.cpp', 'r').read().split('\n')
#print code
rounds=[]
inblock=False
blockcount=-1 # no bug here
block=[]
for line in code:
    begin_round_pattern = '^    for'
    match = re.search(begin_round_pattern, line)
    if match:
        inblock=True
        blockcount+=1
        block.append('')
        if blockcount==4:
            rounds.append(block)
            blockcount=0
            block=[]
    if inblock:
        lookup_pattern='(memory|out)\[.* = T'
        match=re.search(lookup_pattern, line)
        if match:
            block.append(line.strip())

rounds.append(block)

r=1
for block in reversed(rounds):
    print '//ROUND%i' % r
    r+=1
    for line in block:
        print line
