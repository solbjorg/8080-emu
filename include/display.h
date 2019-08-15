#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#define WIDTH		256
#define HEIGHT		224

typedef struct {
	SDL_Window *	window;
	SDL_Renderer *	renderer;
} SdlContext;

SdlContext *new_display();
void render(SdlContext *context, uint8_t *vram);
void free_display(SdlContext *context);

#endif
