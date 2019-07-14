#include "operations.h"

void lxi_pair(uint8_t *first_reg, uint8_t *second_reg, uint8_t low_data, uint8_t high_data) {
	*first_reg = high_data;
	*second_reg = low_data;
}

void lxi_sp(uint16_t *sp, uint8_t low_data, uint8_t high_data) {
	*sp = low_data + (uint16_t)high_data << 8;
}

void mvi_reg(uint8_t *reg, uint8_t data) {
	*reg = data;
}
