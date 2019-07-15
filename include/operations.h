#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <stdint.h>
#include "types.h"

// LXI operation on SP and a register pair, respectively.
void lxi_sp(uint16_t *sp, uint8_t low_data, uint8_t high_data);
void lxi_pair(uint8_t *first_reg, uint8_t *second_reg, uint8_t low_data, uint8_t high_data);

void mvi_reg(uint8_t *reg, uint8_t data);

#endif
