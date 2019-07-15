#include "main.h"

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
	printf("Above operation is not yet supported.\n");
	print_state(state);
	exit(0);
}

int decode_op(state *state) {
	uint8_t *instruction = &state->memory[state->regs->pc];

	printf("%04x: ", state->regs->pc);
	disassemble_current_opcode(instruction);

	uint8_t op_width = 1;

	if (is_lxi(instruction[0])) { // begin lxi
		enum reg pair = resolve_pair_sp(instruction[0] >> 4);
		switch (pair)
		{
		case B:
			lxi_pair(&state->regs->b, &state->regs->c, instruction[1], instruction[2]);
			break;

		case D:
			lxi_pair(&state->regs->d, &state->regs->e, instruction[1], instruction[2]);
			break;

		case H:
			lxi_pair(&state->regs->h, &state->regs->l, instruction[1], instruction[2]);
			break;

		case SP:
			lxi_sp(&state->regs->sp, instruction[1], instruction[2]);
			break;

		default:
			fprintf(stderr, "Malformed lxi expression.");
		}
		op_width = 3;
	} else if (is_jmp(instruction[0])) { // begin jmp
		bool condition;
		if (instruction[0] == 0xc3) {
			condition = true; // JMP
		} else {
			switch ((instruction[0] & 0x40) >> 3)
			{
			case 0:
				condition = !state->flags->z; // JNZ
				break;

			case 1:
				condition = state->flags->z; // JZ
				break;

			case 2:
				condition = !state->flags->c; // JNC
				break;

			case 3:
				condition = state->flags->c; // JC
				break;

			case 4:
				condition = !state->flags->p; // JPO
				break;

			case 5:
				condition = state->flags->p; // JPE
				break;

			case 6:
				condition = !state->flags->s; // JP
				break;

			case 7:
				condition = state->flags->s; // JM
				break;
			}
		}
		op_width = jmp_condition(condition, &state->regs->pc, (uint16_t)(instruction[2]<<8)+instruction[1]);
	} else if (is_mvi(instruction[0])) { // begin mvi
		enum reg r = resolve_reg(instruction[0] >> 3);
		if (r == M) {
			mvi_mem(state);
		} else {
			uint8_t *reg = get_reg(r, state->regs);
			mvi_reg(reg, instruction[1]);
		}
		op_width = 2;
	} else if (is_push(instruction[0])) {
		enum reg pair = resolve_pair_psw(instruction[0] >> 4);
		if (pair == A) {
			// TODO
		} else {
			uint8_t *stack = &state->memory[state->regs->sp];
			switch (pair)
			{
			case B:
				push(state->regs->b, state->regs->c, stack);
				break;

			case D:
				push(state->regs->d, state->regs->e, stack);
				break;

			case H:
				push(state->regs->h, state->regs->l, stack);
				break;

			case A:
				push(state->regs->a, get_psw(state->flags), stack);
				break;

			default:
				fprintf(stderr, "Malformed push expression.");
			}
			state->regs->sp -= 2;
		}
	} else {
		switch (instruction[0])
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
