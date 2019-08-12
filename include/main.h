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
#include <SDL2/SDL.h>

#define SWAP(x, y)    do { typeof(x) SWAP = x; x = y; y = SWAP; } while (0)
#define WIDTH		256
#define HEIGHT		224

// end points of memory regions
#define ROM		0x0
#define RAM		0x2000
#define VRAM		0x2400

bool is_mvi(uint8_t opcode);
bool is_lxi(uint8_t opcode);

void unimplemented_op(state *state);
uint8_t decode_op(state *state);

#endif
