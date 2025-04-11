#ifndef KEYEXPANSION_H
#define KEYEXPANSION_H

#define Nb 4

#include <stdint.h>
#include "SubBytes.h"

static const uint32_t rcon[10] = {(0x01)<<24, (0x02)<<24, (0x04)<<24, (0x08)<<24, (0x10)<<24, (0x20)<<24, (0x40)<<24, (0x80)<<24, (0x1b)<<24, (0x36)<<24};

void KeyExpansion(uint8_t *, uint32_t *, int);

#endif