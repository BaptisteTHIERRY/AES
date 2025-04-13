#ifndef CBC_H
#define CBC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "KeyExpansion.h"
#include "AES.h"
#include "blockXOR.h"

void CBC_encrypt( FILE* fileIn,  FILE* fileOut,  uint8_t* key,  int Nk, uint8_t IV[16]);
void CBC_decrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk, uint8_t IV[16]);


#endif