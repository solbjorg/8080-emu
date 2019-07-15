#include "main.h"

bool is_mvi(uint8_t opcode) {
	return (opcode & 0b11000111) == 6;
}

bool is_lxi(uint8_t opcode) {
	return (opcode & 0b11001111) == 1;
}

int main(int argc, char *argv[]) {
	state *state = new_state(0xffff);
	uint16_t length = 0;

	if (argc < 2) {
		puts("No file supplied.");
		return 1;
	}
	FILE *f = fopen(argv[1], "r");
	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		if (state->memory) {
			fread(state->memory, 1, length, f);
		}
		fclose(f);
	} else {
		puts("File not found.");
		return 1;
	}
	while (state->regs->pc < length) {
		state->regs->pc += decode_op(state);
	}
	free_state(state);
	return 0;
}

void unimplemented_op(state *state) {
	printf("Operation is not yet supported:\n");
	disassemble_current_opcode(state);
	exit(0);
}

int decode_op(state *state) {
	uint8_t opcode = state->memory[state->regs->pc];
	uint8_t op_width = 1;

	if (is_lxi(opcode)) {
		enum reg pair = resolve_pair(opcode >> 4);
		switch (pair)
		{
		case B:
			lxi_pair(&state->regs->b, &state->regs->a, state->memory[state->regs->pc + 1],
			    state->memory[state->regs->pc + 2]);
			break;

		case D:
			lxi_pair(&state->regs->d, &state->regs->e, state->memory[state->regs->pc + 1],
			    state->memory[state->regs->pc + 2]);
			break;

		case H:
			lxi_pair(&state->regs->h, &state->regs->l, state->memory[state->regs->pc + 1],
			    state->memory[state->regs->pc + 2]);
			break;

		case SP:
			lxi_sp(&state->regs->sp, state->memory[state->regs->pc + 1], state->memory[state->regs->pc + 2]);
			break;
		}
		op_width = 3;
	} else if (is_mvi(opcode)) {
		enum reg r = resolve_reg(opcode >> 3);
		if (r == M) {
			mvi_mem(state);
		} else {
			uint8_t *reg = get_reg(r, state->regs);
			mvi_reg(reg, state->memory[state->regs->pc + 1]);
		}
		op_width = 2;
	} else {
		switch (opcode)
		{
		case 0x00:
		case 0x08:
		case 0x10:
		case 0x18:
		case 0x20:
		case 0x28:
		case 0x30:
		case 0x38:
		case 0xcb:
		case 0xd9:
		case 0xdd:
		case 0xed:
		case 0xfd: // NOP
			break;

		default:
			unimplemented_op(state);
		}
	}
	return op_width;
}
