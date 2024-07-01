#include "../headers/header.h"
#include <SDL2/SDL_image.h>

#define NUM_TEXTURES 5
SDL_Texture* textures[NUM_TEXTURES];

void loadTextures(SDL_Renderer* renderer) {
	const char* textureFiles[NUM_TEXTURES] = {
		"textures/bricksx34.png",
		"textures/redbrick.png",
		"textures/eagle.png",
		"textures/graystone.png",
		"textures/wood.png"
	};

	for (int i = 0; i < NUM_TEXTURES; i++) {
		SDL_Surface* surface = IMG_Load(textureFiles[i]);
		if (!surface) {
			printf("Failed to load texture: %s\n", IMG_GetError());
			exit(1);
		}
		textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
}

void unloadTextures() {
	for (int i = 0; i < NUM_TEXTURES; i++) {
		SDL_DestroyTexture(textures[i]);
	}
}
