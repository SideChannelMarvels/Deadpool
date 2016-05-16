#!/usr/bin/env python

import os
import sys
import frida
import psutil

# In case of infinite loops, deadpool_dfa may have killed previous run of this script
# but not the spawned process so let's make it sure by ourselves:
for proc in psutil.process_iter():
    if proc.name() == 'drmless':
        proc.kill()
os.chmod('drmless', 0o755)
pid=frida.spawn(['drmless'])
session=frida.attach(pid)
# IDA -> aes_wb_decryptor is at 0x08298E7C
# $ nm drmless |grep wb
# 08298e7c T aes_wb_decryptor

# there is a 0x400 bytes buffer available in .bss at 0x082A5600 (attr)
# $ nm -S drmless|grep " b "
# ...
# 082a5600 00000400 b attr

scriptsrc="""
'use strict';
var aes_wb_decryptor = new NativeFunction(ptr(0x08298E7C), 'void', ['pointer', 'pointer']);
var buf=ptr(0x082A5600)
Memory.writeByteArray(buf, [0x74, 0x65, 0x73, 0x74, 0x74, 0x65, 0x73, 0x74, 0x74, 0x65, 0x73, 0x74, 0x74, 0x65, 0x73, 0x74])
aes_wb_decryptor(buf, buf)
console.log(Memory.readByteArray(buf, 16))
"""
script = session.create_script(scriptsrc)
script.load()
# 00 01 02 03 04 05 06 07 00 01 02 03 04 05 06 07
# ea 45 aa 04 e9 6f cf 1c e9 22 f9 19 7a 3e a3 db
