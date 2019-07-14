#ifndef MAIN_H
#define MAIN_H

#include "operations.h"
#include "resolves.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_mvi(uint8_t opcode);
bool is_lxi(uint8_t opcode);

void print_state(registers *state);

void unimplemented_op(uint8_t opcode);
int decode_op(registers *state, uint8_t *memory);

#endif
