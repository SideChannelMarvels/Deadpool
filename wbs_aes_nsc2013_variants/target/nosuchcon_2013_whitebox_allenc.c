#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

unsigned char is_hex_char(char c)
{
    return (
        (c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F')
    );
}
int main(int argc, char *argv[])
{
    struct aes_wb_s aes;
    uint8_t i, round;
    uint8_t t[12][16];
    uint8_t m[16];

    for(i = 0; i < 32; i += 2)
    {
        if(is_hex_char(argv[1][i]) == 0 || is_hex_char(argv[1][i + 1]) == 0)
            return EXIT_FAILURE;
        unsigned char str_bytes[3] = {
            argv[1][i],
            argv[1][i + 1],
            0
        };
        m[ i / 2] = strtoul((const char*)str_bytes, NULL, 16);
    }


    FILE* f;
    f = fopen("wbt_allenc", "rb");
    fread(&aes, 1, sizeof(aes), f);
    fclose(f);

    printf("Input:    ");
    for (i = 0; i < 16; i++)
        printf("%02X ", m[i]);
    printf("\n");
    for (i = 0; i < 16; i++)
        t[0][(i%4u)*4 + (i/4u)] = aes.initSub.inv_sub[m[i]];

    printf("Enc in:   ");
    for (i = 0; i < 16; i++)
        printf("%02X ", t[0][i]);
    printf("\n");
    for (round = 1; round < 10; round ++)
    {
        for (i = 0; i < 16; i++)
        {
            uint8_t b[4];
            uint8_t j;
            for (j = 0; j < 4; j++)
                b[j] = aes.roundTables[round-1][i][j][t[round-1][j*4+((i+j)%4u)]];
            t[round][i] = aes.xorTables[2][(aes.xorTables[0][(b[0]<<8)|b[1]] << 8) | aes.xorTables[1][(b[2]<<8)|b[3]]];
        }
    }
    for (i = 0; i < 16; i++)
        t[10][i/4u + (i%4u)*4] = aes.finalTable[i][t[9][(i&(~3)) +((i+i/4)%4u)]];

    printf("Enc out:  ");
    for (i = 0; i < 16; i++)
        printf("%02X ", t[10][i]);
    printf("\n");

    printf("Output:   ");
    for (i = 0; i < 16; i++)
        printf("%02X ", aes.finalSub.inv_sub[t[10][i]]);
    printf("\n");

    return 0;
}
