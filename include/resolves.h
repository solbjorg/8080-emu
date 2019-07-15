#ifndef RESOLVES_H
#define RESOLVES_H

#include "operations.h"
#include <stdint.h>
#include <stdio.h>

/**
 * Resolves various opcodes, usually the ones that contain addressing within 1 byte, e.g. mov instructions.
 */

enum reg {
	B, C, D, E, H, L, M, A, SP
};

bool is_mvi(uint8_t opcode);
bool is_lxi(uint8_t opcode);
bool is_push(uint8_t opcode);

/**
 * Takes in (lowest) 3 bits, resolves it as an enum reg (excluding SP).
 * Used for opcodes with 3 bits that specify the register, e.g. MVI.
 */
enum reg resolve_reg(uint8_t reg);

/**
 * Takes in (lowest) 2 bits, returns the register pair those map to.
 * Used for opcodes with 2 bits that specify a pair, e.g. LXI, where the
 * last pair is SP and PSW (A) respectively.
 */
enum reg resolve_pair_sp(uint8_t reg);
enum reg resolve_pair_psw(uint8_t reg);

uint8_t *get_reg(enum reg reg, registers *const state);
uint8_t get_psw(flags *flags);

#endif
