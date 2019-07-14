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
