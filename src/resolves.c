#include "resolves.h"

inline bool is_push(uint8_t opcode) {
	return (opcode & 0xcf) == 0xc5;
}

inline bool is_mvi(uint8_t opcode) {
	return (opcode & 0xc7) == 0x06;
}

inline bool is_lxi(uint8_t opcode) {
	return (opcode & 0xcf) == 0x01;
}

enum reg resolve_reg(uint8_t reg) {
	return (enum reg)(reg & 7);
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

uint8_t *get_reg(enum reg reg, registers *const regs) {
	switch (reg)
	{
	case B:
		return &regs->b;

	case C:
		return &regs->c;

	case D:
		return &regs->d;

	case E:
		return &regs->e;

	case H:
		return &regs->h;

	case L:
		return &regs->l;

	case A:
		return &regs->a;

	default:
		fprintf(stderr, "Invalid register (e.g. M, SP) given to get_reg.\n");
		return 0;
	}
}

uint8_t get_psw(flags *flags) {
	// puts flags together into expected representation of PSW, as used by a bunch of instructions.
	return flags->c + (1 << 1) + (flags->p << 2) + (flags->ac << 4) + (flags->z << 6) + (flags->s << 7);
}
