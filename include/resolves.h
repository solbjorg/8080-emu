#ifndef RESOLVES_H
#define RESOLVES_H
#include <stdint.h>

/**
 * Resolves various opcodes, usually the ones that contain addressing within 1 byte, e.g. mov instructions.
 */

enum reg {
	B, C, D, E, H, L, M, A, SP
};

/* Takes in (lowest) 3 bits, returns the register those map to. */
enum reg resolve_reg(uint8_t reg);

/* Takes in (lowest) 2 bits, returns the register pair those map to. */
enum reg resolve_pair(uint8_t reg);

#endif
