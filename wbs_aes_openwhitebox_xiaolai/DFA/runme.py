#!/usr/bin/env python3

import os
import sys
import ctypes
import argparse
import struct
from darkphoenixAES import WhiteBoxedAES, Attack
import multiprocessing as mp

class whiteboxProxy(WhiteBoxedAES):

    def __init__(self, libpath, keyFile):
        self.libpath = os.path.abspath(libpath)
        self.keyfile = os.path.abspath(keyFile)

        self.setupLib()

        self.fault = {}

    def setupLib(self):
        self.libraryHandle = ctypes.cdll.LoadLibrary(os.path.abspath(self.libpath))

        self.parseKeyData = self.libraryHandle.parseKeyData
        self.runWhiteBox = self.libraryHandle.runWhiteBox
        self.shiftRows = self.libraryHandle.shiftRows
        self.mixColumns = self.libraryHandle.mixColumns

        self.parseKeyData.argtypes = [ctypes.c_char_p, ctypes.c_int]
        self.runWhiteBox.argtypes = [ctypes.c_char_p, ctypes.c_int]
        self.shiftRows.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
        self.mixColumns.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]

        self.loadKeys(self.keyfile)

        self.inputBuffer = ctypes.create_string_buffer(b"", 16)

    def __getstate__(self):
        """Return state values to be pickled."""
        return (self.libpath, self.keyfile, self.fault)

    def __setstate__(self, state):
        """Restore state from the unpickled state values."""
        self.libpath, self.keyfile, self.fault = state

        self.setupLib()

    def loadKeys(self, keyfile):

        with open(keyfile, 'rb') as f:
            data = f.read()

        self.parseKeyData(data, len(data))

    def writeInputBuffer(self, data):
        self.inputBuffer.raw = data

    def readInputBuffer(self):
        return self.inputBuffer.raw

    def getRoundNumber(self):
        return 10

    def isEncrypt(self):
        return True

    def hasReverse(self):
        return False

    def apply(self, data):
        self.writeInputBuffer(data)

        self.runWhiteBox(self.inputBuffer, 16)

        return self.readInputBuffer()

    def applyFault(self, data, faults):
        assert len(faults) == 1

        fround, fbytes, fxorvalue = faults[0]

        self.writeInputBuffer(data)

        for i in range(10):

            if i == fround:
                self.performFault(fround, fbytes, fxorvalue)
            else:
                self.shiftRows(self.inputBuffer, 16, i)
            self.mixColumns(self.inputBuffer, 16, i)

        self.shiftRows(self.inputBuffer, 16, 10)

        return self.readInputBuffer()

    def applyShiftRow(self, data, roundN):

        self.writeInputBuffer(bytes(data))
        self.shiftRows(self.inputBuffer, 16, roundN)
        return self.readInputBuffer()

    def diffResult(self, ref, out):
        # search after ShiftRow the changed byte (on 2byte encoding)

        diffPos = []
        for pos, (x, y) in enumerate(zip(ref, out)):
            if x != y:
                diffPos.append(pos)
        finalDiff = []
        for fbytes in range(0, 16, 2):
            if fbytes in diffPos or (fbytes+1) in diffPos:
                finalDiff.append(fbytes)
                finalDiff.append(fbytes+1)

        return finalDiff

    def mutateInput(self, data, fbytes, n):
        pos = fbytes - (fbytes % 4)

        v = struct.unpack('<I', data[pos:pos+4])[0] ^ n
        return data[:pos] + struct.pack("<I", v) + data[pos+4:]


    def performFault(self, roundN, fbytes, fxorvalue):
        baseInput = self.readInputBuffer()
        if roundN == 0:
            res = list(baseInput)
            res[fbytes] ^= fxorvalue
            self.writeInputBuffer(bytes(res))
            self.shiftRows(self.inputBuffer, 16, roundN)
            return

        if (baseInput, roundN, fbytes) not in self.fault:
            vref = self.applyShiftRow(baseInput, roundN)

            diffPos = []
            n = 0
            while len(diffPos) != 8:
                n += 1
                diffPos = self.diffResult(vref,
                                self.applyShiftRow(
                                    self.mutateInput(baseInput, fbytes, n),
                                    roundN))

            outChangeByte = diffPos[(fbytes % 4) * 2]
            assert outChangeByte + 1 == diffPos[(fbytes % 4) * 2 + 1]

            outVal = [ vref[outChangeByte:outChangeByte+2] ]

            n = 0
            while len(outVal) < 256:
                n += 1
                newOut = self.applyShiftRow(
                                self.mutateInput(baseInput, fbytes, n),
                                roundN)[outChangeByte:outChangeByte+2]
                if newOut not in outVal:
                    outVal.append(newOut)
            self.fault[(baseInput, roundN, fbytes)] = (outChangeByte, vref, outVal)

        outChangeByte, vref, outVal = self.fault[(baseInput, roundN, fbytes)]

        res = vref[:outChangeByte] + outVal[fxorvalue] + vref[outChangeByte+2:]

        self.writeInputBuffer(res)


def run():

    parser = argparse.ArgumentParser()

    parser.add_argument("--key", type=str, default="key.txt")
    parser.add_argument("--lib", type=str, default="libwbxiaolai.so")
    parser.add_argument("--saveFile", type=str, default="result_backup.json")

    args = parser.parse_args()

    wb = whiteboxProxy(args.lib, args.key)

    # go C library doesn't like to support fork, reload it in each now thread
    mp.set_start_method('spawn')

    attack = Attack(wb)
    attack.run(args.saveFile)
    attack.printKey()
    print("key:", attack.getKey().hex())


if __name__ == "__main__":
    run()
