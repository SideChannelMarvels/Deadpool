#include <assert.h>
#include <stdio.h>
#include <string.h>


void AES_128_encrypt(unsigned char ciphertext[16], unsigned char plaintext[16]);

void
print_bytes(unsigned char bytes[16]) {
    for (int i = 0; i < 16; ++i) {
        printf("%02x", bytes[i]);
    }
    printf("\n");
}

int
main(int argc, char **argv) {
    assert(argc == 2);
    assert(strlen(argv[1]) == 32);

    unsigned char plaintext[16];
    unsigned char ciphertext[16];

    char *pos = argv[1];

    for (int count = 0; count < 16; count++) {
        sscanf(pos, "%2hhx", &plaintext[count]);
        pos += 2;
    }

    AES_128_encrypt(ciphertext, plaintext);
    print_bytes(ciphertext);
}
