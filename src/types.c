#include "types.h"

void print_state(state const *const state) {
	printf("Registers:\na: %02x, b: %02x, c: %02x, d: %02x, e: %02x, h: %02x, l: %02x, pc: %04x, sp: %04x\n",
	    state->regs->a, state->regs->b, state->regs->c, state->regs->d, state->regs->e, state->regs->h, state->regs->l,
	    state->regs->pc, state->regs->sp);
	printf("Flags:\ns: %d, z: %d, ac: %d, p: %d, c: %d\n", state->flags->s, state->flags->z, state->flags->ac,
	    state->flags->p, state->flags->c);
	uint8_t *stack = &state->memory[state->regs->sp];
	printf("Stack:\n[+2: %02x, +1: %02x, +0: %02x, -1: %02x, -2: %02x]\n", stack[2], stack[1], stack[0], stack[-1], stack[-2]);
}

registers *new_registers() {
	registers *regs = malloc(sizeof(registers));

	regs->a = 0;
	regs->b = 0;
	regs->c = 0;
	regs->d = 0;
	regs->e = 0;
	regs->h = 0;
	regs->l = 0;
	regs->pc = 0;
	regs->sp = 0;
	return regs;
}

void free_registers(registers *regs) {
	free(regs);
}

flags *new_flags() {
	flags *flags = malloc(sizeof(flags));

	flags->s = 0;
	flags->z = 0;
	flags->ac = 0;
	flags->p = 0;
	flags->c = 0;
	return flags;
}

void free_flags(flags *flags) {
	free(flags);
}

state *new_state(size_t memory_size) {
	state *state = malloc(sizeof(state));

	state->regs = new_registers();
	state->flags = new_flags();
	state->memory = malloc(sizeof(uint8_t) * memory_size);
	memset(state->memory, 0, sizeof(uint8_t) * memory_size);
	state->inte = false;
	return state;
}

void free_state(state *state) {
	free_registers(state->regs);
	free_flags(state->flags);
	free(state->memory);
	free(state);
}
