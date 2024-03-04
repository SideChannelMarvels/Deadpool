#!/usr/bin/env python3

from bluegalaxyenergy import BGE
from nosuchcon_2013_whitebox import NSCWhiteBoxedAES


def test():
    mywb = NSCWhiteBoxedAES()

    # run the attack
    bge = BGE(mywb)
    # Direct attempt fails with an advise to use 'run(..., shuffle=True)'
    # bge.run(roundList=[2, 3, 4, 5])
    bge.run(roundList=[2, 3, 4, 5], shuffle=True)

    # extract the key from the available roundKeys
    key = bge.computeKey()
    if key is not None:
        print("key:", key.hex())
        # State was actually not shuffled but simply transposed:
        print("states shufflings:")
        for k, v in bge.getShuffle().items():
            print(k, v)


if __name__ == "__main__":
    test()
