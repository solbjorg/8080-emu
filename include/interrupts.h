#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <time.h>
#include "types.h"
#include "operations.h"

typedef struct {
  time_t timer1;
  time_t timer2;
} timers;

void generate_interrupt(state* state, uint8_t irq);
void trigger_interrupts(timers* timers, state* state);

timers* new_timers();
void free_timers(timers* timers);

#endif
