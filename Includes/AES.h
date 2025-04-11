#ifndef AES_H
#define AES_H

#define Nb 4

#include "ShiftRows.h"
#include "SubBytes.h"
#include "MixColumns.h"
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void AES_encrypt(const uint8_t* in, uint8_t* out, const uint32_t* w, const int NK);
void AES_decrypt(const uint8_t* in, uint8_t* out, const uint32_t* w,const int Nk);

#endif