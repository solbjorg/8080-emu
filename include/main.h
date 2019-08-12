#ifndef MAIN_H
#define MAIN_H

#include "operations.h"
#include "resolves.h"
#include "types.h"
#include "disassembler.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SWAP(x, y)    do { typeof(x) SWAP = x; x = y; y = SWAP; } while (0)

bool is_mvi(uint8_t opcode);
bool is_lxi(uint8_t opcode);

void unimplemented_op(state *state);
int decode_op(state *state);

#endif
