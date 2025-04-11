#ifndef MIXCOLUMNS_H
#define MIXCOLUMNS_H

#define MASK_HEAD_BIT (1<<7) // Most significant bit of a byte
#define M 0x1b // The irreducible polynomial of AES M = x^8 + x^4 + x^3 + x + 1

#include <stdint.h>

void MixColumns(uint8_t [4][4]);

#endif