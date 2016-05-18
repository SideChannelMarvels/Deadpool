#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Attacking Round 3 (actually any round between 2 and 9)
#define ROUND 3

typedef struct sub_s {
    uint8_t sub[0x100];
    uint8_t inv_sub[0x100];
} *sub_t;

typedef struct aes_wb_s {
    struct sub_s initSub;
    struct sub_s finalSub;
    uint8_t xorTables[3][0x10000];
    uint8_t roundTables[9][16][4][0x100];
    uint8_t finalTable[16][0x100];
} *aes_wb_t;

typedef struct aes_wb_s_noenc_nokey {
    uint8_t xorTables[3][0x10000];
    uint8_t roundTables[9][16][4][0x100];
    uint8_t finalTable[16][0x100];
} *aes_wb_t_noenc_nokey;

// bidirectional keyschedule to derive round keys from any round key
// with some code hijacked from https://github.com/kokke/tiny-AES128-C
#define Nb 4
#define Nk 4
#define KEYLEN 16
#define Nr 10
static uint8_t RoundKey[176];
static uint8_t Key[16];
static const uint8_t sbox[256] =   {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

// The round constant word array, Rcon[i], contains the values given by 
// x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
// Note that i starts at 1, not 0).
static const uint8_t Rcon[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states. 
void KeyExpansion(uint8_t start)
{
  uint32_t i, j, k;
  uint8_t tempa[4]; // Used for the column/row operations
  start *=Nk;
  // The first round key is the key itself.
  for(i = start; i < (Nk+start); ++i)
  {
    RoundKey[(i * 4) + 0] = Key[((i-start) * 4) + 0];
    RoundKey[(i * 4) + 1] = Key[((i-start) * 4) + 1];
    RoundKey[(i * 4) + 2] = Key[((i-start) * 4) + 2];
    RoundKey[(i * 4) + 3] = Key[((i-start) * 4) + 3];
  }
  // All other round keys are found from the previous round keys.
  for(; (i < (Nb * (Nr + 1))); ++i)
  {
    for(j = 0; j < 4; ++j)
    {
      tempa[j]=RoundKey[(i-1) * 4 + j];
    }
    if (i % Nk == 0)
    {
      // This function rotates the 4 bytes in a word to the left once.
      // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

      // Function RotWord()
      {
        k = tempa[0];
        tempa[0] = tempa[1];
        tempa[1] = tempa[2];
        tempa[2] = tempa[3];
        tempa[3] = k;
      }

      // SubWord() is a function that takes a four-byte input word and 
      // applies the S-box to each of the four bytes to produce an output word.

      // Function Subword()
      {
        tempa[0] = sbox[tempa[0]];
        tempa[1] = sbox[tempa[1]];
        tempa[2] = sbox[tempa[2]];
        tempa[3] = sbox[tempa[3]];
      }

      tempa[0] =  tempa[0] ^ Rcon[i/Nk];
    }
    else if (Nk > 6 && i % Nk == 4)
    {
      // Function Subword()
      {
        tempa[0] = sbox[tempa[0]];
        tempa[1] = sbox[tempa[1]];
        tempa[2] = sbox[tempa[2]];
        tempa[3] = sbox[tempa[3]];
      }
    }
    RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ tempa[0];
    RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ tempa[1];
    RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ tempa[2];
    RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ tempa[3];
  }
  for(i=start+3; i>3; i--)
  {
    for(j = 0; j < 4; ++j)
    {
      tempa[j]=RoundKey[(i-1) * 4 + j];
    }
    if (i % Nk == 0)
    {
      // This function rotates the 4 bytes in a word to the left once.
      // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

      // Function RotWord()
      {
        k = tempa[0];
        tempa[0] = tempa[1];
        tempa[1] = tempa[2];
        tempa[2] = tempa[3];
        tempa[3] = k;
      }

      // SubWord() is a function that takes a four-byte input word and 
      // applies the S-box to each of the four bytes to produce an output word.

      // Function Subword()
      {
        tempa[0] = sbox[tempa[0]];
        tempa[1] = sbox[tempa[1]];
        tempa[2] = sbox[tempa[2]];
        tempa[3] = sbox[tempa[3]];
      }

      tempa[0] =  tempa[0] ^ Rcon[i/Nk];
    }
    else if (Nk > 6 && i % Nk == 4)
    {
      // Function Subword()
      {
        tempa[0] = sbox[tempa[0]];
        tempa[1] = sbox[tempa[1]];
        tempa[2] = sbox[tempa[2]];
        tempa[3] = sbox[tempa[3]];
      }
    }
    RoundKey[(i - Nk) * 4 + 0] = RoundKey[i * 4 + 0] ^ tempa[0];
    RoundKey[(i - Nk) * 4 + 1] = RoundKey[i * 4 + 1] ^ tempa[1];
    RoundKey[(i - Nk) * 4 + 2] = RoundKey[i * 4 + 2] ^ tempa[2];
    RoundKey[(i - Nk) * 4 + 3] = RoundKey[i * 4 + 3] ^ tempa[3];
  }
  for(j = 0; j < 16*11; ++j)
  {
    if (j%16==0)
      printf("K%02i: ", j/16);
    printf("%02X", RoundKey[j]);
    if (j%16==15)
      printf("\n");
  }
}

int main(int argc, char *argv[])
{
    struct aes_wb_s aes;
    struct aes_wb_s_noenc_nokey aes_noenc_nokey;
    uint8_t i, ii,round;
    uint8_t t[12][16];
    uint8_t kandidates[16][256];
    uint8_t kandidates_n[16];
    uint8_t kandidates_tmp[16][256];
    uint8_t kandidates_tmp_n[16];

    FILE* f;
    f = fopen("../LIFT/wbt_rom", "rb");
    fread(&aes, 1, sizeof(aes), f);
    fclose(f);

    f = fopen("wbt_noenc_nokey", "rb");
    fread(&aes_noenc_nokey, 1, sizeof(aes_noenc_nokey), f);
    fclose(f);

    round=ROUND;
    uint16_t m[256];
    uint8_t mc;
    mc=0x00;
    // Let's guess encoding of value mc=0x00 in xorSubs[2], the encoding table used between rounds
    //   Encoding of mc is m[mc]
    // Find the corresponding key of 2nd round (so the 3rd key)
    for (m[mc]=0;m[mc]<256;m[mc]++)
    {
//        printf("Hypothesis %02X is encoded as %02X\n", mc, m[mc]);
    // There are 16 roundTables for each round, taking 4 bytes as input and producing one byte
    // (one roundTables here is actually the 4 sub-roundTables combined with the 3 xor tables)
        for (ii = 0; ii < 16; ii++)
        {
            uint8_t b[4];
            uint16_t n=0;
            uint16_t j=0;
            uint8_t mt=m[mc];
            uint16_t k;
            // demangle index
            i=(ii%4u)*4 + ii/4u;
//            printf("i=%02i ", i);
            kandidates_n[i]=0;
    // To attack one roundTable, we fix the 4 input bytes as m[mc]:m[mc]:m[mc]:m[mc] so the decoded input is supposed to be 00:00:00:00
    // We compute the encoded output byte out and compare it with m[mc]
    //   if out != m[mc], we create a new input out:m[mc]:m[mc]:m[mc]
    //   and chain executions of that roundTable till out==m[mc] so decoded out==00
            do {
                b[0] = aes.roundTables[round-1][i][0][mt];
                b[1] = aes.roundTables[round-1][i][1][m[mc]];
                b[2] = aes.roundTables[round-1][i][2][m[mc]];
                b[3] = aes.roundTables[round-1][i][3][m[mc]];
                mt = aes.xorTables[2][(aes.xorTables[0][(b[0]<<8)|b[1]] << 8) | aes.xorTables[1][(b[2]<<8)|b[3]]];
                n++;
            } while (mt != m[mc]);
//            printf("n=%02X ", n);
//            printf("k= ");
    // The required number of iterations can be compared with a clean implementation
    //   without encodings to check which k candidates require the same number of iterations
            for (k=0; k<256; k++) {
                mt=mc;
                for (j=0;j<n;j++) {
                    b[0] = aes_noenc_nokey.roundTables[round-1][i][0][mt];
                    b[1] = aes_noenc_nokey.roundTables[round-1][i][1][mc];
                    b[2] = aes_noenc_nokey.roundTables[round-1][i][2][mc];
                    b[3] = aes_noenc_nokey.roundTables[round-1][i][3][mc];
                    mt = aes_noenc_nokey.xorTables[2][(aes_noenc_nokey.xorTables[0][(b[0]<<8)|b[1]] << 8) | aes_noenc_nokey.xorTables[1][(b[2]<<8)|b[3]]];
                    mt ^=k;
                }
                if (mt==mc) {
//                    printf("%02X ", k);
                    kandidates[i][kandidates_n[i]++]=k;
                }
            }
//            printf("\n");
        }
//        printf("\n");

    // Then we can take a second guess of encoding value mc=0x01 and filter the k candidates to keep those which are compatible
    // And so on. If no k candidates is left for one of the round key bytes, we backtrack our encoding guesses
        uint16_t mcnext;
        for (mcnext=0x01;mcnext<256;mcnext++)
        {

            for (m[mcnext]=0;m[mcnext]<256;m[mcnext]++)
            {
//                printf("Hypothesis %02X is encoded as %02X\n", mcnext, m[mcnext]);
                for (ii = 0; ii < 16; ii++)
                {
                    uint8_t b[4];
                    uint16_t n=0;
                    uint16_t j=0;
                    uint8_t mt=m[mcnext];
                    uint16_t k;
                    // demangle index
                    i=(ii%4u)*4 + ii/4u;
//                    printf("i=%02i ", i);
                    kandidates_tmp_n[i]=0;
                    do {
                        b[0] = aes.roundTables[round-1][i][0][mt];
                        b[1] = aes.roundTables[round-1][i][1][m[mcnext]];
                        b[2] = aes.roundTables[round-1][i][2][m[mcnext]];
                        b[3] = aes.roundTables[round-1][i][3][m[mcnext]];
                        mt = aes.xorTables[2][(aes.xorTables[0][(b[0]<<8)|b[1]] << 8) | aes.xorTables[1][(b[2]<<8)|b[3]]];
                        n++;
                    } while (mt != m[mcnext]);
//                    printf("n=%02X ", n);
//                    printf("k= ");
                    uint8_t ki;
                    for (ki=0; ki<kandidates_n[i]; ki++) {
                        mt=mcnext;
                        k=kandidates[i][ki];
                        for (j=0;j<n;j++) {
                            b[0] = aes_noenc_nokey.roundTables[round-1][i][0][mt];
                            b[1] = aes_noenc_nokey.roundTables[round-1][i][1][mcnext];
                            b[2] = aes_noenc_nokey.roundTables[round-1][i][2][mcnext];
                            b[3] = aes_noenc_nokey.roundTables[round-1][i][3][mcnext];
                            mt = aes_noenc_nokey.xorTables[2][(aes_noenc_nokey.xorTables[0][(b[0]<<8)|b[1]] << 8) | aes_noenc_nokey.xorTables[1][(b[2]<<8)|b[3]]];
                            mt ^=k;
                        }
                        if (mt==mcnext) {
//                            printf("%02X ", k);
                            kandidates_tmp[i][kandidates_tmp_n[i]++]=k;
                        }
                    }
//                    printf("\n");
                    if (kandidates_tmp_n[i]==0)
                        break;
                }
                if (kandidates_tmp_n[i]==0)
                    continue;
//                printf("Copying candidates\n");
                memcpy(kandidates, kandidates_tmp, sizeof(kandidates));
                memcpy(kandidates_n, kandidates_tmp_n, sizeof(kandidates_n));
                break;
            }
            if (kandidates_tmp_n[i]==0)
                break;
        }
        if (mcnext==256 && (kandidates_tmp_n[i]!=0))
        {
            uint16_t i;
            printf("R%iK candidate: ", round);
            for (i = 0; i < 16; i++)
                printf("%02X", kandidates[(i%4u)*4 + i/4u][0]);
            printf("\n");
            for (i = 0; i < 16; i++)
                Key[i]=kandidates[(i%4u)*4 + i/4u][0];
            KeyExpansion(round);
            printf("aes.xorTables[2] mapping:\n");
            for (i = 0; i < 256; i++) {
                printf("%02X:%02X ", i, m[i]);
                if (i%16==15)
                    printf("\n");
            }
            // we could go on but it appears there is always one single X2 mapping and round key possible
            break;
        }
    }
    return 0;
}
