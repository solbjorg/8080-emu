#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
	uint8_t s : 1;
	uint8_t z : 1;
	uint8_t ac : 1; // I'm going to ignore ac for now, as I don't understand it, and space invaders doesn't use it.
	uint8_t p : 1;
	uint8_t c : 1;
} flags;

typedef struct {
	uint8_t		a;
	uint8_t		b;
	uint8_t		c;      // paired with b
	uint8_t		d;
	uint8_t		e;      // paired with d
	uint8_t		h;
	uint8_t		l;      // paired with h
	uint16_t	pc;
	uint16_t	sp;
} registers;

typedef struct {
	registers *	regs;
	flags *		flags;
	uint8_t *	memory;
	bool		inte; // interrupts enabled
} state;

void print_state(state const *const state);

registers *new_registers();
void free_registers(registers *regs);
flags *new_flags();
void free_flags(flags *flags);
state *new_state(size_t memory_size);
void free_state(state *state);

#endif
