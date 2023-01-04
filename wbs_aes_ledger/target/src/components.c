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

// mixcolumns using precalculated multiplication tables
inline void fastMix4bytes(u8 *a, u8 *b, u8 *c, u8 *d) {
  u8 aa, bb, cc, dd;
  aa = mul2[*a]^mul3[*b]^(*c)^(*d);
  bb = mul2[*b]^mul3[*c]^(*d)^(*a);
  cc = mul2[*c]^mul3[*d]^(*a)^(*b);
  dd = mul2[*d]^mul3[*a]^(*b)^(*c);
  *a = aa;
  *b = bb;
  *c = cc;
  *d = dd;
}

inline void mixcolumns(u8 a[4][4]) {
  int i;
  for(i=0; i<4; ++i)  fastMix4bytes(&(a[0][i]), &(a[1][i]), &(a[2][i]), &(a[3][i]));
}

inline void subAndAdd(u8 a[4][4], u8 mask[4][4], int r) {
   int i, j;
   // Miam la belle Sbox (contains mask + roundkey + crc))
   for(i = 0; i < 4; i++)
      for(j = 0; j < 4; j++) a[i][j] = sboxes[r][i][j][a[i][j]][mask[i][j]] ^ crc[i*4+j];
}

inline void shiftrows(u8 a[4][4]) {
  u8 tmp[4];
  int i, j;

  for(i = 1; i < 4; i++) {
    for(j = 0; j < 4; j++)
      tmp[j] = a[i][(j + i) % 4];
    for(j = 0; j < 4; j++) a[i][j] = tmp[j];
  }
}

inline void addroundkey(u8 a[4][4], u8 k[4][4]) {
   int i, j;
   for(i = 0; i < 4; i++)
   for(j = 0; j < 4; j++) a[i][j] ^= k[i][j];
}

