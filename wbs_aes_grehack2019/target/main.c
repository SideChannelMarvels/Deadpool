// hacky inclusion...
#include "enc.c"

int main(int argc, char **argv)
{
    uint8_t buffer[16];
    char *pos;

    if (argc < 2) {
        printf("Usage: %s 000102030405060708090a0b0c0d0e0f\n", argv[0]);
        return 1;
    }

    pos = argv[1];
    for (size_t count = 0; count < sizeof(buffer)/sizeof(*buffer); count++) {
        sscanf(pos, "%2hhx", &buffer[count]);
        pos += 2;
    }
    encrypt(buffer);
    for (int i = 0; i < 16; i++){
        printf("%02x", buffer[i]);
    }
    printf("\n");
    return 0;
}
