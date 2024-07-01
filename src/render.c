#include "../headers/header.h"

void drawLine(SDL_Renderer* renderer, int x, int y1, int y2, int color) {
	SDL_SetRenderDrawColor(renderer, color, color, color, 255);
	SDL_RenderDrawLine(renderer, x, y1, x, y2);
}
