#ifndef CFB_H
#define CFB_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "KeyExpansion.h"
#include "AES.h"
#include "blockXOR.h"

void CFB_encrypt( FILE* fileIn,  FILE* fileOut,  uint8_t* key,  int Nk, uint8_t IV[16]);
void CFB_decrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk, uint8_t IV[16]);

#endif