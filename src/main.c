#include "operations.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_state(registers *state) {
  printf("a: %d, b: %d, c: %d, d: %d, e: %d, h: %d, l: %d, pc: %d, sp: %d",
         state->a, state->b, state->c, state->d, state->e, state->h, state->l,
         state->pc, state->sp);
}

int main(int argc, char *argv[]) {
  registers *state = malloc(sizeof(registers));
  state =
      &(registers){.a = 0,
                   .b = 0,
                   .c = 0,
                   .d = 0,
                   .e = 0,
                   .h = 0,
                   .l = 0,
                   .pc = 0,
                   .sp = 0,
                   .flags = &(flags){.s = 0, .z = 0, .ac = 0, .p = 0, .c = 0}};
  uint8_t memory[65536];
}

void unimplemented_op(uint8_t opcode) {
  printf("%d is not yet supported.", opcode);
}

void decode_op(registers *state, uint8_t *memory) {
  uint8_t opcode = memory[state->pc];
  uint8_t op_width = 1;
  switch (opcode) {
  case 0x00:
  case 0x08:
  case 0x10:
  case 0x18:
  case 0x20:
  case 0x28:
  case 0x30:
  case 0x38:
  case 0xcb:
  case 0xd9:
  case 0xdd:
  case 0xed:
  case 0xfd: // NOP
    break;
  case 0x01:
  default:
    unimplemented_op(opcode);
  }
  state->pc += op_width;
}
