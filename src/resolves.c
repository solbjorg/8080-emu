#include "resolves.h"

inline bool is_ret(uint8_t opcode) {
	return ((opcode & 0xc6) == 0xc0) || (opcode == 0xc1);
}

inline bool is_mov(uint8_t opcode) {
	return (opcode & 0xc0) == 0x40;
}

inline bool is_dad(uint8_t opcode) {
	return (opcode & 0xcf) == 0x09;
}

inline bool is_inx_or_dcx(uint8_t opcode) {
	return (opcode & 0xc7) == 0x03;
}

inline bool is_inr_or_dcr(uint8_t opcode) {
	return (opcode & 0xc6) == 0x04;
}

inline bool is_call(uint8_t opcode) {
	return ((opcode & 0xc7) == 0xc4) || (opcode == 0xcd);
}

inline bool is_jmp(uint8_t opcode) {
	return ((opcode & 0xc7) == 0xc2) || (opcode == 0xc3);
}

inline bool is_push(uint8_t opcode) {
	return (opcode & 0xcf) == 0xc5;
}

inline bool is_pop(uint8_t opcode) {
	return (opcode & 0xcf) == 0xc1;
}

inline bool is_mvi(uint8_t opcode) {
	return (opcode & 0xc7) == 0x06;
}

inline bool is_lxi(uint8_t opcode) {
	return (opcode & 0xcf) == 0x01;
}

inline bool is_rotate(uint8_t opcode) {
	return (opcode & 0xe7) == 0x07;
}

inline bool is_xra(uint8_t opcode) {
	return (opcode & 0xf8) == 0xa8;
}

enum reg resolve_reg(uint8_t reg) {
	return (enum reg)(reg & 7);
}

bool resolve_condition_jmp_or_call(flags *const flags, uint8_t conditional) {
	bool condition;

	switch (conditional)
	{
	case 0:
		condition = !flags->z;  // JNZ
		break;

	case 1:
		condition = flags->z;  // JZ
		break;

	case 2:
		condition = !flags->c;  // JNC
		break;

	case 3:
		condition = flags->c;  // JC
		break;

	case 4:
		condition = !flags->p;  // JPO
		break;

	case 5:
		condition = flags->p;  // JPE
		break;

	case 6:
		condition = !flags->s;  // JP
		break;

	case 7:
		condition = flags->s; // JM
		break;
	}
	return condition;
}

enum reg resolve_pair_sp(uint8_t reg) {
	reg &= 3;

	enum reg ref;
	switch (reg)
	{
	case 0:
		ref = B;
		break;

	case 1:
		ref = D;
		break;

	case 2:
		ref = H;
		break;

	case 3:
		ref = SP;
		break;
	}
	return ref;
}

enum reg resolve_pair_psw(uint8_t reg) {
	reg &= 3;

	enum reg ref;
	switch (reg)
	{
	case 0:
		ref = B;
		break;

	case 1:
		ref = D;
		break;

	case 2:
		ref = H;
		break;

	case 3:
		ref = A;
		break;
	}
	return ref;
}

uint8_t *get_reg(enum reg reg, state *const state) {
	switch (reg)
	{
	case B:
		return &state->regs->b;

	case C:
		return &state->regs->c;

	case D:
		return &state->regs->d;

	case E:
		return &state->regs->e;

	case H:
		return &state->regs->h;

	case L:
		return &state->regs->l;

	case A:
		return &state->regs->a;

	case M:
		return &state->memory[get_pair_value(H, state)];

	default:
		fprintf(stderr, "Invalid register (e.g. M, SP) given to get_reg.\n");
		return 0;
	}
}

uint8_t get_psw(flags *flags) {
	// puts flags together into expected representation of PSW, as used by a bunch of instructions.
	return flags->c + (1 << 1) + (flags->p << 2) + (flags->ac << 4) + (flags->z << 6) + (flags->s << 7);
}

void set_psw(uint8_t values, flags *flags) {
	flags->c = (values & 1);
	flags->p = (values & (1 << 2)) >> 2;
	flags->ac = (values & (1 << 4)) >> 4;
	flags->z = (values & (1 << 6)) >> 6;
	flags->s = (values & (1 << 7)) >> 7;
}

void set_flags(uint16_t result, flags *flags) {
	flags->p = !(result % 2);
	flags->s = (result & (1 << 7)) >> 7;
	flags->z = result == 0;
}

uint16_t get_pair_value(enum reg pair, state *const state) {
	switch (pair)
	{
	case B:
		return (uint16_t)(state->regs->b << 8) + state->regs->c;

	case D:
		return (uint16_t)(state->regs->d << 8) + state->regs->e;

	case H:
		return (uint16_t)(state->regs->h << 8) + state->regs->l;

	case A:
		return (uint16_t)(state->regs->a << 8) + get_psw(state->flags);

	default:
		fprintf(stderr, "Malformed get_pair_value, pair: %d\n", pair);
	}
}

void set_pair_value(enum reg pair, uint16_t value, state *state) {
	switch (pair)
	{
	case B:
		state->regs->b = value >> 8;
		state->regs->c = value & 0xff;
		break;

	case D:
		state->regs->d = value >> 8;
		state->regs->e = value & 0xff;
		break;

	case H:
		state->regs->h = value >> 8;
		state->regs->l = value & 0xff;
		break;

	case A:
		state->regs->a = value >> 8;
		set_psw(value & 0xff, state->flags);
		break;

	default:
		fprintf(stderr, "Malformed get_pair_value, pair: %d\n", pair);
	}
}
