#!/usr/bin/env python3

from darkphoenixAES import Attack
from nosuchcon_2013_whitebox import NSCWhiteBoxedAES

def test():
    a = Attack(NSCWhiteBoxedAES())
    a.run('backup.json')
    a.printKey()
    print("key:", a.getKey().hex())

if __name__ == "__main__":
    test()
