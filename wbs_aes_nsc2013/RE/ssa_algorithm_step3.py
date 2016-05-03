#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re

print """
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "0vercl0k_writeup/wbaes128.h"

typedef struct sub_s {
    uint8_t sub[0x100];
    uint8_t inv_sub[0x100];
} *sub_t;

typedef struct aes_wb_s {
    struct sub_s initSub;
    struct sub_s finalSub;
    uint8_t xorTables[3][0x10000];
    uint8_t roundTables[9][16][4][0x100];
    uint8_t finalTable[16][0x100];
} *aes_wb_t;

int main(int argc, char *argv[])
{
    struct aes_wb_s aes;
    FILE* f;
"""

f = open('tables', 'r')
for line in f.readlines():
    patternfT = 'fT\[([0-9]+)\] = (.*)'
    patternrT = 'rT([0-9]+)\[([0-9]+)\]\[([0-9]+)\] = (.*)'
    patternxT = 'xT([0-9]+) = (.*)'

    match = re.search(patternfT, line)
    if match:
        print "    memcpy(aes.finalTable[%s], %s, sizeof(%s));" % (match.group(1), match.group(2), match.group(2))

    match = re.search(patternrT, line)
    if match:
        print "    memcpy(aes.roundTables[%s][%s][%s], %s, sizeof(%s));" % (match.group(1), match.group(2), match.group(3), match.group(4), match.group(4))

    match = re.search(patternxT, line)
    if match:
        print "    memcpy(aes.xorTables[%s], %s, sizeof(%s));" % (match.group(1), match.group(2), match.group(2))

print """
    f = fopen("result/wbt_nsc", "wb");
    fwrite(&aes, 1, sizeof(aes), f);
    fclose(f);
}
"""
