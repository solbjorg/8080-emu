#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

uint8_t disassemble_current_opcode(uint8_t *const instruction);

#endif
