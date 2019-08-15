#ifndef ISA_H
#define ISA_H

#include "operations.h"
#include "resolves.h"
#include "types.h"
#include "disassembler.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SWAP(x, y)    do { typeof(x) SWAP = x; x = y; y = SWAP; } while (0)

// end points of memory regions
#define ROM		0x0
#define RAM		0x2000
#define VRAM		0x2400
#define RAM_MIRROR	0x4000
void unimplemented_op(state *state);
uint8_t decode_op(state *state);

#endif
