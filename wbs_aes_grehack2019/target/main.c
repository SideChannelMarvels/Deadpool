#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tables.h"

void
encrypt (uint8_t *buffer)
{
  uint8_t s[42];

  memcpy(s, buffer, 16);

  #include "instr.c"

  memcpy(buffer, s, 16);
}

int main(int argc, char **argv)
{
	int i;
	uint8_t buffer[16];
	char *pos;

	if (argc < 2) {
		return 0;
	}

	pos = argv[1];
	for (size_t count = 0; count < sizeof buffer/sizeof *buffer; count++) {
        	sscanf(pos, "%2hhx", &buffer[count]);
        	pos += 2;
    	}
	encrypt (buffer);
	for (i=0;i<16;i++){
		printf("%02x", buffer[i]);
	}
	printf("\n");
	return 0;
}
