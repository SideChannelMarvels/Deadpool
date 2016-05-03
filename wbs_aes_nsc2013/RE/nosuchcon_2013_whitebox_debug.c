#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    uint8_t i, round;
    for (i = 0; i < 16; i++){
        printf("t[0][%i] = iSinv[%i]\n", (i%4u)*4 + (i/4u), i);
    }
    // black magic...
    for (round = 1; round < 10; round ++)
    {
        for (i = 0; i < 16; i++)
        {
            uint8_t b[4];
            uint8_t j;
            for (j = 0; j < 4; j++) {
                printf("b%i_%i[%i] = rT%i[%i][%i][t[%i][%i]]\n", round, i, j, round-1, i, j, round-1, j*4+((i+j)%4u));
            }
            printf("b%i_%i_01 = xT0[b%i_%i[0]][b%i_%i[1]]\n", round, i, round, i, round, i);
            printf("b%i_%i_23 = xT1[b%i_%i[2]][b%i_%i[3]]\n", round, i, round, i, round, i);
            printf("t[%i][%i] = xT2[b%i_%i_01][b%i_%i_23]\n", round, i, round, i, round, i);
        }
    }
    for (i = 0; i < 16; i++) {
        printf("t[10][%i] = fT[%i][t[9][%i]]\n", i/4u + (i%4u)*4, i, (i&(~3)) +((i+i/4)%4u));
    }

    return 0;
}
