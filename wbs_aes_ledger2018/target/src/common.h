/*******************************************************************************
*   Ledger CTF 2018 - Security Team
*   (c) 2018 - Ledger
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/


#ifndef _COMMON_H_
#define _COMMON_H_

typedef unsigned char u8;
#define NCIPHER 15
#define ENDCIPHER 40

// Just to keep track of the scheduling of each cipher
int scheduling[NCIPHER];

// current mask. The same for all the encryption /!\ change this ?
u8 mask[4][4];

// mask ^ mixcolumns(mask) => xor it with Mixco(s + mask) => Mixco(s) + mask
u8 mask3[4][4];

// original mask shifted to remove the mask at the end
u8 shiftedmask[4][4];
// unsigned char zecrc[16] = {0x9b,0x04,0x57,0x44,0xe5,0x1f,0xf0,0xf9,0xe8,0x4a,0x54,0xda,0x68,0xe0,0x52,0xb8};

// crc computed by the main. If it's wrong, the tables change and the encryption is wrong
unsigned char crc[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// dummy: it's used to patch the binary and change it to retrieve the wanted crc:
u8 dum[16] = {0x12,0x34,0x56,0x78,0x9a,0xBC,0xDE,0xF0,0x21,0x43,0x65,0x98,0xBA,0xDC,0xFE,0x34};

// Contains the current state of all the ongoing encryption
u8 states[NCIPHER][4][4];


// Constant table for Mixco
u8 mul2[256]={
  0x00,0x02,0x04,0x06,0x08,0x0A,0x0C,0x0E,0x10,0x12,0x14,0x16,0x18,0x1A,0x1C,0x1E,
  0x20,0x22,0x24,0x26,0x28,0x2A,0x2C,0x2E,0x30,0x32,0x34,0x36,0x38,0x3A,0x3C,0x3E,
  0x40,0x42,0x44,0x46,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x56,0x58,0x5A,0x5C,0x5E,
  0x60,0x62,0x64,0x66,0x68,0x6A,0x6C,0x6E,0x70,0x72,0x74,0x76,0x78,0x7A,0x7C,0x7E,
  0x80,0x82,0x84,0x86,0x88,0x8A,0x8C,0x8E,0x90,0x92,0x94,0x96,0x98,0x9A,0x9C,0x9E,
  0xA0,0xA2,0xA4,0xA6,0xA8,0xAA,0xAC,0xAE,0xB0,0xB2,0xB4,0xB6,0xB8,0xBA,0xBC,0xBE,
  0xC0,0xC2,0xC4,0xC6,0xC8,0xCA,0xCC,0xCE,0xD0,0xD2,0xD4,0xD6,0xD8,0xDA,0xDC,0xDE,
  0xE0,0xE2,0xE4,0xE6,0xE8,0xEA,0xEC,0xEE,0xF0,0xF2,0xF4,0xF6,0xF8,0xFA,0xFC,0xFE,
  0x1B,0x19,0x1F,0x1D,0x13,0x11,0x17,0x15,0x0B,0x09,0x0F,0x0D,0x03,0x01,0x07,0x05,
  0x3B,0x39,0x3F,0x3D,0x33,0x31,0x37,0x35,0x2B,0x29,0x2F,0x2D,0x23,0x21,0x27,0x25,
  0x5B,0x59,0x5F,0x5D,0x53,0x51,0x57,0x55,0x4B,0x49,0x4F,0x4D,0x43,0x41,0x47,0x45,
  0x7B,0x79,0x7F,0x7D,0x73,0x71,0x77,0x75,0x6B,0x69,0x6F,0x6D,0x63,0x61,0x67,0x65,
  0x9B,0x99,0x9F,0x9D,0x93,0x91,0x97,0x95,0x8B,0x89,0x8F,0x8D,0x83,0x81,0x87,0x85,
  0xBB,0xB9,0xBF,0xBD,0xB3,0xB1,0xB7,0xB5,0xAB,0xA9,0xAF,0xAD,0xA3,0xA1,0xA7,0xA5,
  0xDB,0xD9,0xDF,0xDD,0xD3,0xD1,0xD7,0xD5,0xCB,0xC9,0xCF,0xCD,0xC3,0xC1,0xC7,0xC5,
  0xFB,0xF9,0xFF,0xFD,0xF3,0xF1,0xF7,0xF5,0xEB,0xE9,0xEF,0xED,0xE3,0xE1,0xE7,0xE5
};

u8 mul3[256]={
  0x00,0x03,0x06,0x05,0x0C,0x0F,0x0A,0x09,0x18,0x1B,0x1E,0x1D,0x14,0x17,0x12,0x11,
  0x30,0x33,0x36,0x35,0x3C,0x3F,0x3A,0x39,0x28,0x2B,0x2E,0x2D,0x24,0x27,0x22,0x21,
  0x60,0x63,0x66,0x65,0x6C,0x6F,0x6A,0x69,0x78,0x7B,0x7E,0x7D,0x74,0x77,0x72,0x71,
  0x50,0x53,0x56,0x55,0x5C,0x5F,0x5A,0x59,0x48,0x4B,0x4E,0x4D,0x44,0x47,0x42,0x41,
  0xC0,0xC3,0xC6,0xC5,0xCC,0xCF,0xCA,0xC9,0xD8,0xDB,0xDE,0xDD,0xD4,0xD7,0xD2,0xD1,
  0xF0,0xF3,0xF6,0xF5,0xFC,0xFF,0xFA,0xF9,0xE8,0xEB,0xEE,0xED,0xE4,0xE7,0xE2,0xE1,
  0xA0,0xA3,0xA6,0xA5,0xAC,0xAF,0xAA,0xA9,0xB8,0xBB,0xBE,0xBD,0xB4,0xB7,0xB2,0xB1,
  0x90,0x93,0x96,0x95,0x9C,0x9F,0x9A,0x99,0x88,0x8B,0x8E,0x8D,0x84,0x87,0x82,0x81,
  0x9B,0x98,0x9D,0x9E,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8F,0x8C,0x89,0x8A,
  0xAB,0xA8,0xAD,0xAE,0xA7,0xA4,0xA1,0xA2,0xB3,0xB0,0xB5,0xB6,0xBF,0xBC,0xB9,0xBA,
  0xFB,0xF8,0xFD,0xFE,0xF7,0xF4,0xF1,0xF2,0xE3,0xE0,0xE5,0xE6,0xEF,0xEC,0xE9,0xEA,
  0xCB,0xC8,0xCD,0xCE,0xC7,0xC4,0xC1,0xC2,0xD3,0xD0,0xD5,0xD6,0xDF,0xDC,0xD9,0xDA,
  0x5B,0x58,0x5D,0x5E,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4F,0x4C,0x49,0x4A,
  0x6B,0x68,0x6D,0x6E,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7F,0x7C,0x79,0x7A,
  0x3B,0x38,0x3D,0x3E,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2F,0x2C,0x29,0x2A,
  0x0B,0x08,0x0D,0x0E,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1F,0x1C,0x19,0x1A
};

u8 rcon[16]={ 0x8d, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab};

#endif
