#!/usr/bin/env python3

from bluegalaxyenergy import BGE
from nosuchcon_2013_whitebox import NSCWhiteBoxedAES

def test():
    mywb = NSCWhiteBoxedAES()

    # run the attack
    bge = BGE(mywb)
    bge.run(roundList=[2,3,4,5])

    # extract the key from the available roundKeys
    key = bge.computeKey()
    if key is not None:
        print("key:", key.hex())
    # for this specific whitebox, returned roundkeys were transposed
    # therefore if at least two roundkeys were found, previous computeKey() failed
    # and we need to indicate the round keys were transposed:
    key = bge.computeKey(transposed_rk=True)
    if key is not None:
        print("key:", key.hex())

if __name__ == "__main__":
    test()
