#!/usr/bin/env python3

import argparse
import pyqbdi
import os
import sys
import ctypes
from bluegalaxyenergy import WhiteBoxedAES, BGE

class Extractor(WhiteBoxedAES):

    def __init__(self, libraryPath):
        self.stackSize = 0x10000
        self.libraryPath = libraryPath

        self.vm = None
        self.stackptr = None
        self.baseStackPtr = None
        self.inputBuffer = None

        self.writePosition = []
        self.writeAddress = set()

        self.internalBufferAddress = None
        self.breakpoints = None
        self.requestedRound = None
        self.applyRoundData = None

    # encode buffer
    def setBuffer(self, value):
        assert len(value) <= 16
        pyqbdi.writeMemory(self.inputBuffer, value)

    def readBuffer(self):
        return pyqbdi.readMemory(self.inputBuffer, 16)

    def loadLibrary(self, libraryPath):

        self.libraryHandle = ctypes.cdll.LoadLibrary(os.path.abspath(libraryPath))
        address = ctypes.cast(self.libraryHandle.encrypt, ctypes.c_void_p).value

        # check the address is executable
        for mem in pyqbdi.getCurrentProcessMaps():
            if address < mem.range.start or mem.range.end <= address:
                continue
            assert (mem.permission & pyqbdi.PF_EXEC) != 0, "Address is not executable"
            return address
        assert False, "encrypt address is not mapped"

    # use static method to not specify self as the first argument
    @staticmethod
    def writeCBK(vm, gpr, fpr, self):

        memaccess = vm.getInstMemoryAccess()
        for acc in memaccess:
            if (self.stackptr <= acc.accessAddress and
                    acc.accessAddress < self.stackptr+self.stackSize and
                    acc.size == 1):
                self.writePosition.append(acc)
                self.writeAddress.add(acc.accessAddress)
        return pyqbdi.CONTINUE

    def prepareVM(self):
        # create VM
        self.vm = pyqbdi.VM()

        # allocate a dedicate stack and backup RSP (must be restore before each
        # execution to have the same stack address)
        self.stackptr = pyqbdi.allocateVirtualStack(self.vm.getGPRState(), self.stackSize)
        self.baseStackPtr = self.vm.getGPRState().REG_SP

        # allocate the parameter of `encrypt` and set a dummy content
        self.inputBuffer = pyqbdi.allocateMemory(16)
        self.setBuffer(bytes(range(16)))

        # import shared library
        self.encryptAddress = self.loadLibrary(self.libraryPath)
        self.vm.addInstrumentedModuleFromAddr(self.encryptAddress)

        # add callback for memory access
        self.vm.addMemRangeCB(self.stackptr, self.stackptr + self.stackSize,
                              pyqbdi.MEMORY_WRITE, self.writeCBK, self)

        # restore RSP
        gpr = self.vm.getGPRState()
        gpr.REG_SP = self.baseStackPtr
        self.vm.setGPRState(gpr)

        # collect write position
        self.vm.call(self.encryptAddress, [self.inputBuffer])
        self.vm.deleteAllInstrumentations()

        # search the internal buffer position and the breakpoint address
        candidates = {}
        for cand in self.writeAddress:
            isValid = True
            for i in range(16):
                isValid &= ((cand+i) in self.writeAddress)
            if not isValid:
                continue

            # search position where the address is write, then address+1, ...
            instAddresses = []
            bufferWritePosition = 0
            for pos, acc in enumerate(self.writePosition):
                if acc.accessAddress == cand + bufferWritePosition:
                    bufferWritePosition += 1
                    if bufferWritePosition == 16:
                        instAddresses.append(acc.instAddress)
                        bufferWritePosition = 0
                elif cand <= acc.accessAddress and acc.accessAddress < cand+16:
                    bufferWritePosition = 0

            if len(instAddresses) > 0:
                candidates[cand] = instAddresses

        assert len(candidates) == 1
        self.internalBufferAddress = list(candidates.keys())[0]
        self.breakpoints = candidates[self.internalBufferAddress]
        assert len(self.breakpoints) == 10

        # insert the breakpoint in the VM
        for pos in self.breakpoints:
            self.vm.addCodeAddrCB(pos, pyqbdi.InstPosition.POSTINST,
                                  self.breakpointCBK, self)

    @staticmethod
    def breakpointCBK(vm, gpr, fpr, self):

        if self.requestedRound == 0:
            pyqbdi.writeMemory(self.internalBufferAddress, self.applyRoundData)
        elif self.requestedRound == -1:
            self.applyRoundData = pyqbdi.readMemory(self.internalBufferAddress, 16)
            return pyqbdi.STOP

        self.requestedRound -= 1
        return pyqbdi.CONTINUE

    def getRoundNumber(self):
        return 10

    def applyRound(self, data, roundN):

        if self.vm is None:
            self.prepareVM()

        assert self.internalBufferAddress is not None

        # set breakpoint parameter for this execution
        self.requestedRound = roundN - 1
        self.applyRoundData = data

        # restore RSP
        gpr = self.vm.getGPRState()
        gpr.REG_SP = self.baseStackPtr
        self.vm.setGPRState(gpr)

        # execute `encrypt` with breakpoint
        self.vm.call(self.encryptAddress, [self.inputBuffer])

        return self.applyRoundData

def run():

    parser = argparse.ArgumentParser()

    parser.add_argument("whiteboxLibrary", type=str,
                        help="sharedLibrary of the whitebox")

    args = parser.parse_args()

    bge = BGE(Extractor(args.whiteboxLibrary))
    bge.run([1, 2, 3, 4, 5, 6])

    key = bge.computeKey()
    assert key is not None
    print("key:", key.hex())

if __name__ == "__main__":
    run()
