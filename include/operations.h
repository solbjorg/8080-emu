#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <stdint.h>

typedef struct {
	uint8_t s : 1;
	uint8_t z : 2;
	uint8_t ac : 2;
	uint8_t p : 2;
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
	flags *		flags;
} registers;

// LXI operation on SP and a register pair, respectively.
void lxi_sp(uint16_t *sp, uint8_t low_data, uint8_t high_data);
void lxi_pair(uint8_t *first_reg, uint8_t *second_reg, uint8_t low_data, uint8_t high_data);

#endif
