#ifndef GCM_H
#define GCM_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "KeyExpansion.h"
#include "AES.h"
#include "blockXOR.h"

void GCM_encrypt(FILE* fileIn, FILE* fileOut, uint8_t* key, int Nk, uint8_t* IV, uint8_t* AD, int lenAD);
void GCM_decrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk, uint8_t* IV, uint8_t* AD, int lenAD);

#endif