#include "main.h"

int main(int argc, char *argv[]) {
	state *state = new_state(0xffff);
	SdlContext *context = new_display();
	timers* timers = new_timers();
	uint16_t length = 0;

	if (argc < 2) {
		puts("No file supplied.");
		return 1;
	}
	FILE *f = fopen(argv[1], "r");
	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		if (state->memory) {
			fread(state->memory, 1, length, f);
		}
		fclose(f);
	} else {
		puts("File not found.");
		return 1;
	}

	while (state->regs->pc < length) {
		uint8_t op_width = decode_op(state);
		if (op_width == 255) { // temporary. TODO.
			break;
		} else {
			state->regs->pc += op_width;
		}
		trigger_interrupts(timers, state);
		render(context, &state->memory[VRAM]);
	}

	free_display(context);
	free_state(state);
	free_timers(timers);
	return 0;
}
