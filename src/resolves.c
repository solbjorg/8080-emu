#include "resolves.h"
enum reg resolve_reg(uint8_t reg) {
	reg &= 7;
	enum reg ref;
	switch (reg)
	{
	case 0:
		ref = B;
		break;

	case 1:
		ref = C;
		break;

	case 2:
		ref = D;
		break;

	case 3:
		ref = E;
		break;

	case 4:
		ref = H;
		break;

	case 5:
		ref = L;
		break;

	case 6:
		ref = M;
		break;

	case 7:
		ref = A;
		break;
	}
	return ref;
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

uint8_t *get_reg(enum reg reg, registers *state) {
	switch (reg)
	{
	case B:
		return &state->b;

	case C:
		return &state->c;

	case D:
		return &state->d;

	case E:
		return &state->e;

	case H:
		return &state->h;

	case L:
		return &state->l;

	case A:
		return &state->a;

	default:
		fprintf(stderr, "Invalid register (e.g. M, SP) given to get_reg.\n");
		return 0;
	}
}
