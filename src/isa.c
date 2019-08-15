#include "isa.h"

void unimplemented_op(state *state) {
	printf("Above operation is not yet supported.\n");
	print_state(state);
}

uint8_t decode_op(state *state) {
	uint8_t *instruction = &state->memory[state->regs->pc];

	printf("%04x: ", state->regs->pc);
	disassemble_current_opcode(instruction);

	uint8_t op_width = 1;
	if (state->regs->pc == 0x143d) {
		print_state(state);
	}

	if (is_mov(instruction[0])) { // begin mov
		enum reg src_reg = resolve_reg(instruction[0]);
		enum reg dst_reg = resolve_reg(instruction[0] >> 3);
		if ((src_reg == M) && (dst_reg == M)) { // MOV M, M == HLT
			// HLT here.
		} else {
			uint8_t *src = get_reg(src_reg, state);
			uint8_t *dst = get_reg(dst_reg, state);
			if (dst_reg == M) {
				write_to_memory(state, get_pair_value(H, state), *src);
			} else {
				*dst = *src;
			}
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
			condition = resolve_condition_jmp_or_call(state->flags, (instruction[0] & 0x3f) >> 3);
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
			push(state->regs->b, state->regs->c, state);
			break;

		case D:
			push(state->regs->d, state->regs->e, state);
			break;

		case H:
			push(state->regs->h, state->regs->l, state);
			break;

		case A:
			push(state->regs->a, get_psw(state->flags), state);
			break;

		default:
			fprintf(stderr, "Malformed push expression.");
			exit(0);
		}
		state->regs->sp -= 2;
	} else if (is_pop(instruction[0])) { // begin POP
		enum reg pair = resolve_pair_psw(instruction[0] >> 4);
		uint8_t *stack = &state->memory[state->regs->sp];
		switch (pair)
		{
		case B:
			pop(&state->regs->b, &state->regs->c, stack);
			break;

		case D:
			pop(&state->regs->d, &state->regs->e, stack);
			break;

		case H:
			pop(&state->regs->h, &state->regs->l, stack);
			break;

		case A:
		{
			uint8_t psw;
			pop(&state->regs->a, &psw, stack);
			set_psw(psw, state->flags);
			break;
		}

		default:
			fprintf(stderr, "Malformed POP expression.");
			exit(0);
		}
		state->regs->sp += 2;
	} else if (is_dad(instruction[0])) { // begin dad. untested
		enum reg pair = resolve_pair_sp(instruction[0] >> 4);
		uint16_t hl = get_pair_value(H, state);
		uint16_t rp = get_pair_value(pair, state);
		uint16_t result;
		if (addition_will_overflow_16(hl, rp)) {
			// overflow
			result = 0xffff;
			state->flags->c = 1;
		} else {
			result = hl + rp;
			state->flags->c = 0;
		}
		set_pair_value(H, result, state);
	} else if (is_ret(instruction[0])) { // ret
		bool condition;
		if (instruction[0] == 0xc1) {
			condition = true; // RET
		} else {
			condition = resolve_condition_jmp_or_call(state->flags, (instruction[0] & (1 << 3)) >> 3);
		}
		jmp_condition(condition, &state->regs->pc, (uint16_t)(state->memory[state->regs->sp+1]<<8)+state->memory[state->regs->sp]);
		if (condition) {
			state->regs->sp += 2;
		}
	} else if (is_rotate(instruction[0])) {
		uint8_t hi = state->regs->a >> 7;
		uint8_t lo = state->regs->a & 0x01;
		switch (instruction[0] >> 3)
		{
		case 0:         // RLC
			state->regs->a <<= 1;
			state->regs->a += hi;
			state->flags->c = hi;
			break;

		case 1:         // RRC
			state->regs->a >>= 1;
			state->regs->a += lo << 7;
			state->flags->c = lo;
			break;

		case 2:         // RAL
			state->regs->a <<= 1;
			state->regs->a += state->flags->c;
			state->flags->c = hi;
			break;

		case 3:         // RAR
			state->regs->a >>= 1;
			state->regs->a += state->flags->c << 7;
			state->flags->c = lo;
			break;
		}
	} else if (is_xra(instruction[0])) {
		uint8_t *reg = get_reg(resolve_reg(instruction[0] & 0xf7), state);
		*reg ^= state->regs->a;
		state->flags->c = 0;
		set_flags(*reg, state->flags);
	} else if (is_ana(instruction[0])) {
		uint8_t *reg = get_reg(resolve_reg(instruction[0] & 0xf7), state);
		*reg &= state->regs->a;
		state->flags->c = 0;
		set_flags(*reg, state->flags);
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
			write_to_memory(state, (uint16_t)(state->regs->b << 8) + state->regs->c, state->regs->a);
			break;

		case 0x0a: // LDAX B
			state->regs->a = state->memory[(uint16_t)(state->regs->b << 8) + state->regs->c];
			break;

		case 0x12: // STAX D
			write_to_memory(state, (uint16_t)(state->regs->d << 8) + state->regs->e, state->regs->a);
			break;

		case 0x1a: // LDAX D
			state->regs->a = state->memory[(uint16_t)(state->regs->d << 8) + state->regs->e];
			break;

		case 0x22: // SHLD
			write_to_memory(state, (uint16_t)(instruction[2] << 8) + instruction[1], state->regs->l);
			write_to_memory(state, (uint16_t)(instruction[2] << 8) + instruction[1] + 1, state->regs->h);
			op_width = 3;
			break;

		case 0x2a: // LHLD
			state->regs->l = state->memory[(uint16_t)(instruction[2] << 8) + instruction[1]];
			state->regs->h = state->memory[(uint16_t)(instruction[2] << 8) + instruction[1]+1];
			op_width = 3;
			break;

		case 0x32:      // STA
			write_to_memory(state, (uint16_t)(instruction[2] << 8) + instruction[1], state->regs->a);
			op_width = 3;
			break;

		case 0x3a:      // LDA
			state->regs->a = state->memory[(uint16_t)(instruction[2] << 8) + instruction[1]];
			op_width = 3;
			break;

		case 0xc6: // ADI
			if (addition_will_overflow_8(state->regs->a, instruction[1])) {
				state->regs->a = 0xff;
				state->flags->c = 1;
			} else {
				state->regs->a += instruction[1];
				state->flags->c = 0;
			}
			set_flags(state->regs->a, state->flags);
			op_width = 2;
			print_state(state);
			break;

		case 0xe6:      // ANI
			state->regs->a &= instruction[1];
			state->flags->c = 0;
			set_flags(state->regs->a, state->flags);
			op_width = 2;
			break;

		case 0xeb:      // XCHG
			SWAP(state->regs->h, state->regs->d);
			SWAP(state->regs->l, state->regs->e);
			break;

		case 0xd3:         // OUT - TODO
			op_width = 2;
			break;

		case 0xfe: // CPI
			set_flags(state->regs->a + (~instruction[1]+1), state->flags);
			state->flags->c = instruction[1] > state->regs->a;
			break;

		case 0xf3:
			state->inte = false;
			break;

		case 0xfb:
			state->inte = true;
			break;

		default:
			unimplemented_op(state);
			return 255;
		}
	}
	return op_width;
}
