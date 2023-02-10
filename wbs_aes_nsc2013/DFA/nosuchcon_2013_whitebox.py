from darkphoenixAES import WhiteBoxedAES

class NSCWhiteBoxedAES(WhiteBoxedAES):
    def __init__(self):
        with open("../RE/result/wbt_nsc", "rb") as f:
            self.initSub_sub = list(f.read(0x100))
            self.initSub_inv_sub = list(f.read(0x100))
            self.finalSub_sub = list(f.read(0x100))
            self.finalSub_inv_sub = list(f.read(0x100))
            self.xorTables0 = list(f.read(0x10000))
            self.xorTables1 = list(f.read(0x10000))
            self.xorTables2 = list(f.read(0x10000))
            self.roundTables=[[[None]*4 for _ in range(16)] for _ in range(9)]
            for i in range(9):
                for j in range(16):
                    for k in range(4):
                        self.roundTables[i][j][k] = list(f.read(0x100))
            self.finalTable=[None]*16
            for i in range(16):
                self.finalTable[i] = list(f.read(0x100))

    def getRoundNumber(self):
        return 10

    def isEncrypt(self):
        return True

    def hasReverse(self):
        return False

    def apply(self, data):
        for round in range(10):
            data = self.applyRound(data, round)
        return data

    def applyRound(self, data, roundN):
        output=[None]*16
        if roundN < 9:
            for i in range(16):
                b = [0, 0, 0, 0]
                for j in range(4):
                    b[j] = self.roundTables[roundN][i][j][data[j*4+((i+j)%4)]];
                    output[i] = self.xorTables2[(self.xorTables0[(b[0]<<8)|b[1]] << 8) | self.xorTables1[(b[2]<<8)|b[3]]]
        else:
            for i in range(16):
                output[i//4 + (i%4)*4] = self.finalTable[i][data[(i&(~3)) +((i+i//4)%4)]]
        return output
