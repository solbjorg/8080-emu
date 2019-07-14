#include "main.h"

bool is_mvi(uint8_t opcode) {
	return (opcode & 0b11000111) == 6;
}

bool is_lxi(uint8_t opcode) {
	return (opcode & 0b11001111) == 1;
}

void print_state(registers *state) {
	printf("a: %d, b: %d, c: %d, d: %d, e: %d, h: %d, l: %d, pc: %d, sp: %d", state->a,
	    state->b, state->c, state->d, state->e, state->h, state->l, state->pc, state->sp);
}

int main(int argc, char *argv[]) {
	registers *state = malloc(sizeof(registers));

	state =
	    &(registers) {
		.a = 0,
		.b = 0,
		.c = 0,
		.d = 0,
		.e = 0,
		.h = 0,
		.l = 0,
		.pc = 0,
		.sp = 0,
		.flags = NULL
	};
	state->flags = malloc(sizeof(flags));
	state->flags = &(flags) {
		.s = 0, .z = 0, .ac = 0, .p = 0, .c = 0
	};
	uint8_t *memory = malloc(sizeof(uint8_t)*0xffff);
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
		if (memory) {
			fread(memory, 1, length, f);
		}
		fclose(f);
	} else {
		puts("File not found.");
		return 1;
	}
	while (state->pc < length) {
		state->pc += decode_op(state, memory);
	}
	return 0;
}

void unimplemented_op(uint8_t opcode) {
	//printf("%d is not yet supported.\n", opcode);
}

int decode_op(registers *state, uint8_t *memory) {
	uint8_t opcode = memory[state->pc];
	uint8_t op_width = 1;

	if (is_lxi(opcode)) {
		enum reg pair = resolve_pair(opcode >> 4);
		switch (pair)
		{
		case B:
			lxi_pair(&state->b, &state->a, memory[state->pc + 1], memory[state->pc + 2]);
			break;

		case D:
			lxi_pair(&state->d, &state->e, memory[state->pc + 1], memory[state->pc + 2]);
			break;

		case H:
			lxi_pair(&state->h, &state->l, memory[state->pc + 1], memory[state->pc + 2]);
			break;

		case SP:
			lxi_sp(&state->sp, memory[state->pc + 1], memory[state->pc + 2]);
			break;
		}
		op_width = 3;
	} else if (is_mvi(opcode)) {
		enum reg r = resolve_reg(opcode >> 3);
		if (r == M) {
			// Not done yet
		} else {
			uint8_t *reg = get_reg(r, state);
			mvi_reg(reg, memory[state->pc + 1]);
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
			unimplemented_op(opcode);
		}
	}
	return op_width;
}
