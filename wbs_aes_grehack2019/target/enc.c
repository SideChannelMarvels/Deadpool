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

