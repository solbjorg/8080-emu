#include "resolves.h"

enum reg resolve_reg(uint8_t reg) {
	return (enum reg)(reg & 7);
}

enum reg resolve_pair(uint8_t reg) {
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
