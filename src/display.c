#include "display.h"

SdlContext *new_display() {
	SdlContext *context = malloc(sizeof(SdlContext));

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &context->window, &context->renderer);

	return context;
}

void render(SdlContext *context, uint8_t *vram) {
	SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 0);
	SDL_RenderClear(context->renderer);
	SDL_SetRenderDrawColor(context->renderer, 255, 255, 255, 255);
	for (uint16_t h = 0; h < HEIGHT; h++)
	{
		for (uint16_t w = 0; w < WIDTH/8; w++)
		{
			uint8_t byte = vram[(WIDTH/8)*h+w];
			for (uint8_t i = 8; i > 0; i--)
			{
				if ((byte >> (i-1)) & 1) {
					SDL_RenderDrawPoint(context->renderer, w*8+i, h);
				}
			}
		}
	}
	SDL_RenderPresent(context->renderer);
}

void free_display(SdlContext *context) {
	SDL_DestroyRenderer(context->renderer);
	SDL_DestroyWindow(context->window);
	SDL_Quit();
	free(context);
}
