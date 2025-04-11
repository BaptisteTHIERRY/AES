#ifndef ECB_H
#define ECB_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "KeyExpansion.h"
#include "AES.h"

void ECB_encrypt( FILE* fileIn,  FILE* fileOut,  uint8_t* key,  int Nk);
void ECB_decrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk);

#endif