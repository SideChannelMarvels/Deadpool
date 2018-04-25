#!/usr/bin/env python3

import lief

libnative = lief.parse("./libnative-lib.so")

# Change Symbol version (All to global)
for s in libnative.dynamic_symbols:
    if s.has_version:
        if s.symbol_version.value > 1:
            s.symbol_version.value = 1

# 1. Remove liblog (Android specific)
liblog     = libnative.get_library("liblog.so")
liblog.tag = lief.ELF.DYNAMIC_TAGS.NULL

# 2. Change library names ACCORDING TO YOUR LINUX VERSION USED (here ArchLinux & Debian)
libnative.get_library("libc.so").name = "libc.so.6"
libnative.get_library("libm.so").name = "libm.so.6"

libnative.write("./libnative-lib.so")
