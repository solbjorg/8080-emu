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

	if (is_mov(instruction[0])) { // begin mov
		enum reg src_reg = resolve_reg(instruction[0]);
		enum reg dst_reg = resolve_reg(instruction[0] >> 3);
		if ((src_reg == M) && (dst_reg == M)) { // MOV M, M == HLT
			// HLT here.
		} else {
			uint8_t *src = get_reg(src_reg, state);
			uint8_t *dst = get_reg(dst_reg, state);
			*dst = *src;
		}
	} else if (is_inx_or_dcx(instruction[0])) {             // begin inx/dcx
		// inefficient. TODO.
		bool is_dcx = (instruction[0] & (1 << 3)) >> 3; // qv. programming manual pp. 24
		enum reg pair = resolve_pair_sp(instruction[0] >> 4);
		uint16_t pair_value = get_pair_value(pair, state);
		pair_value += is_dcx ? -1 : 1;
		set_pair_value(pair, pair_value, state);
	} else if (is_inr_or_dcr(instruction[0])) {
		bool is_dcr = instruction[0] & 1;
		uint8_t *reg = get_reg(resolve_reg(instruction[0] >> 3), state);
		*reg += is_dcr ? -1 : 1;
		set_flags(*reg, state->flags);
	} else if (is_lxi(instruction[0])) { // begin lxi
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
			condition = resolve_condition_jmp_or_call(state->flags, (instruction[0] & (1 << 3)) >> 3);
		}
		op_width = jmp_condition(condition, &state->regs->pc, (uint16_t)(instruction[2]<<8)+instruction[1]);
	} else if (is_call(instruction[0])) { // begin call
		bool condition;
		if (instruction[0] == 0xcd) {
			condition = true;
		} else {
			condition = resolve_condition_jmp_or_call(state->flags, instruction[0] >> 3);
		}
		op_width = call_condition(condition, state);
	} else if (is_mvi(instruction[0])) { // begin mvi
		uint8_t *reg = get_reg(resolve_reg(instruction[0] >> 3), state);
		mvi_reg(reg, instruction[1]);
		op_width = 2;
	} else if (is_push(instruction[0])) { // begin push
		enum reg pair = resolve_pair_psw(instruction[0] >> 4);
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
			exit(0);
		}
		state->regs->sp -= 2;
	} else if (is_pull(instruction[0])) { // begin pull TODO
		enum reg pair = resolve_pair_psw(instruction[0] >> 4);
		uint8_t *stack = &state->memory[state->regs->sp];
		switch (pair)
		{
		case B:
			pull(&state->regs->b, &state->regs->c, stack);
			break;

		case D:
			pull(&state->regs->d, &state->regs->e, stack);
			break;

		case H:
			pull(&state->regs->h, &state->regs->l, stack);
			break;

		case A:
		{
			uint8_t psw;
			pull(&state->regs->a, &psw, stack);
			set_psw(psw, state->flags);
			break;
		}

		default:
			fprintf(stderr, "Malformed pull expression.");
			exit(0);
		}
		state->regs->sp += 2;
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

		case 0x02: // STAX B
			state->memory[(uint16_t)(state->regs->b << 8) + state->regs->c] = state->regs->a;
			break;

		case 0x0a: // LDAX B
			state->regs->a = state->memory[(uint16_t)(state->regs->b << 8) + state->regs->c];
			break;

		case 0x12: // STAX D
			state->memory[(uint16_t)(state->regs->d << 8) + state->regs->e] = state->regs->a;
			break;

		case 0x1a: // LDAX D
			state->regs->a = state->memory[(uint16_t)(state->regs->d << 8) + state->regs->e];
			break;

		default:
			unimplemented_op(state);
		}
	}
	return op_width;
}
