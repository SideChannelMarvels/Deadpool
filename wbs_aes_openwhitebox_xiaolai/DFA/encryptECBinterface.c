#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "encryptECBlib.h"

void parseKeyData(char* ptr, size_t len) {
  GoSlice keyBuffer = {ptr, len, len};
  goParseKeyData(&keyBuffer);
}

void runWhiteBox(uint8_t* data, size_t len) {
  GoSlice slice = {data, len, len};
  goRunWhiteBox(&slice);
}

void shiftRows(uint8_t* data, size_t len, size_t round) {
  GoSlice slice = {data, len, len};
  goShiftRows(&slice, round);
}

void mixColumns(uint8_t* data, size_t len, size_t round) {
  GoSlice slice = {data, len, len};
  goMixColumns(&slice, round);
}
