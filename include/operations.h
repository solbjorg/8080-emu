#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <stdint.h>
#include <stdbool.h>
#include "types.h"

// takes a condition, performs jump if it is met; returns amount to skip PC with, which is 0 if jumping and 3 if not.
uint16_t call_condition(bool condition, state *const state);
uint16_t jmp_condition(bool condition, uint16_t *pc, uint16_t addr);

void push(uint8_t first_reg, uint8_t second_reg, uint8_t *stack);
void pull(uint8_t *first_reg, uint8_t *second_reg, uint8_t *stack);

// LXI operation on SP and a register pair, respectively.
void lxi_sp(uint16_t *sp, uint8_t low_data, uint8_t high_data);
void lxi_pair(uint8_t *first_reg, uint8_t *second_reg, uint8_t low_data, uint8_t high_data);

// MVI into a register and memory respectively.
void mvi_reg(uint8_t *reg, uint8_t data);
void mvi_mem(state *state);

#endif
