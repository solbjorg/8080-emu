#include "interrupts.h"

void generate_interrupt(state* state, uint8_t irq) {
  if (state->inte) {
    write_to_memory(state, state->regs->sp - 1, (state->regs->pc + 2) >> 8);
    write_to_memory(state, state->regs->sp - 2, state->regs->pc + 2);
    state->inte = false; // disable interrupts when one is being handled
    state->regs->pc = irq * 8;
  }
}

// TODO everything below is very specific to space invaders. Decouple this kind of logic.
void trigger_interrupts(timers* timers, state* state) {
  time_t now = time(NULL);
  if (now > timers->timer1 + 1/60) {
    timers->timer1 = now;
    generate_interrupt(state, 8);
  } else if (now > timers->timer2 + 1/60) {
    timers->timer2 = now;
    generate_interrupt(state, 10);
  }
}

timers* new_timers() {
  timers* timers = malloc(sizeof(timers));
  timers->timer1 = time(NULL)+1/120;
  timers->timer2 = time(NULL)+1/60;
  return timers;
}

void free_timers(timers* timers) {
  free(timers);
}
