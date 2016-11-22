#!/usr/bin/env python3

import sys
from aes import AES

roundkeys=[
'0D9BE960C438FF85F656BD48B78A0EE2',
'5A769B843C089BB1357FF628AE897D46',
'46359EFF9EFE9E1B41F1AA45CC250A12',
'AFBF8EFAD8F4878B8CF5E498342885FD',
'FF2C650283CF6605B42FB77E624B798D',
'9002F334E6C6288647B873A6145A15C5',
'DB33A8697D32BAA858D9EBB8FD266B3C',
'772DA79020B322D37C9B86DE70F8C3AA',
'600C5335D45052EB3BC418EAD9C611F3',
'3EC60262C17DE8D51F7E92E90715B965',
'4989783DD27A96D30C13C1ED687838F1',
'42ECC85F8C38152BE2D3F70154F12C8F',
'623247F2EA244F65A691A317F74934F2',
'E3A0F7B76729FB15ABE864512923F937',
'4A41F291E18FAEAFEA926076DDD3586B'
]

expandedkey=bytes.fromhex(''.join(roundkeys))

aes=AES()
if len(sys.argv)>1:
    p=bytes(sys.argv[1], encoding='utf8')[:16]
    p=p+b'\00'*(16-len(p))
else:
    p=b"You got broken!!"
try:
    p.decode('utf8')
except UnicodeDecodeError:
    print('ERROR: Please shorten your string!')
    sys.exit()

print("PLAIN: %s" % p.decode('utf8'))
c=bytes(aes.encrypt(p, expandedkey,32))
print("CLIENT ENC %s => %s" % (p.hex(), c.hex()))

p2=bytes(aes.decrypt(c, expandedkey,32))
print("SERVER DEC %s => %s" % (c.hex(), p2.hex()))

p2u=p2.upper()
print("SERVER UP  %s => %s" % (p2.hex(), p2u.hex()))

c3=bytes(aes.encrypt(p2u, expandedkey,32))
print("SERVER ENC %s => %s" % (p2.hex(), c3.hex()))

p3=bytes(aes.decrypt(c3, expandedkey,32))
print("CLIENT DEC %s => %s" % (c3.hex(), p3.hex()))
print("PLAIN: %s" % p3.decode('utf8'))
