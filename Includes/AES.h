#ifndef AES_H
#define AES_H

#define Nb 4

#include "ShiftRows.h"
#include "SubBytes.h"
#include "MixColumns.h"
#include "KeyExpansion.h"
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void AES_cipher(uint8_t*, uint8_t*, uint8_t*, int);

#endif