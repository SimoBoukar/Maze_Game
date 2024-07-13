#include "../headers/header.h"

int main(int argc, char* argv[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_UNDEFINED,
		       	SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	loadTextures(renderer);
	initMap();

	double moveSpeed = 0.1; // Move 5% of a tile per frame
	double rotSpeed = 0.1; // Rotate 5% of a radian per frame

	SDL_bool quit = SDL_FALSE;

	const int targetFPS = 60;
	const int frameDelay = 1000 / targetFPS;

	Uint32 frameStart;
	int frameTime;

	while (!quit) {
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = SDL_TRUE;
			}
		}

		movePlayer(moveSpeed, rotSpeed);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		castRays(renderer);
		renderMinimap(renderer);  // Add this line to render the minimap

		SDL_RenderPresent(renderer);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	unloadTextures();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
