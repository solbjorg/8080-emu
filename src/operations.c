#include "operations.h"

uint16_t jmp_condition(bool condition, uint16_t *pc, uint16_t addr) {
	if (condition) {
		*pc = addr;
		return 0;
	}
	return 3;
}

void lxi_pair(uint8_t *first_reg, uint8_t *second_reg, uint8_t low_data, uint8_t high_data) {
	*first_reg = high_data;
	*second_reg = low_data;
}

void lxi_sp(uint16_t *sp, uint8_t low_data, uint8_t high_data) {
	*sp = (uint16_t)high_data << 8 + low_data;
}

void mvi_reg(uint8_t *reg, uint8_t data) {
	*reg = data;
}

// M = HL; MVI M,FFH <=> store FFH in HL
void mvi_mem(state *state) {
	uint16_t M = (uint16_t)state->regs->h << 8 + state->regs->l;

	state->memory[M] = state->memory[state->regs->pc+1];
}
