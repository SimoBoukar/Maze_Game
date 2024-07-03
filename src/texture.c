#include "../headers/header.h"
#include <SDL2/SDL_image.h>

#define NUM_TEXTURES 5
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

SDL_Texture* textures[NUM_TEXTURES];
Uint32* texturePixels[NUM_TEXTURES];

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

		// Convert surface to RGBA32 format
		SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		SDL_FreeSurface(surface);

		if (!rgbaSurface) {
			printf("Failed to convert texture surface: %s\n", SDL_GetError());
			exit(1);
		}

		// Create texture from surface
		textures[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, 
				SDL_TEXTUREACCESS_STREAMING, 
				TEXTURE_WIDTH, TEXTURE_HEIGHT);
		if (!textures[i]) {
			printf("Failed to create texture: %s\n", SDL_GetError());
			exit(1);
		}

		// Lock texture and copy pixel data
		void* pixels;
		int pitch;
		SDL_LockTexture(textures[i], NULL, &pixels, &pitch);
		memcpy(pixels, rgbaSurface->pixels, rgbaSurface->h * rgbaSurface->pitch);
		SDL_UnlockTexture(textures[i]);

		// Store pixel data for quick access during rendering
		texturePixels[i] = (Uint32*)malloc(TEXTURE_WIDTH * TEXTURE_HEIGHT * sizeof(Uint32));
		memcpy(texturePixels[i], rgbaSurface->pixels, TEXTURE_WIDTH * TEXTURE_HEIGHT * sizeof(Uint32));

		SDL_FreeSurface(rgbaSurface);
	}
}

void unloadTextures() {
	for (int i = 0; i < NUM_TEXTURES; i++) {
		SDL_DestroyTexture(textures[i]);
		free(texturePixels[i]);
	}
}

Uint32 getPixel(int texNum, int x, int y) {
	return texturePixels[texNum][y * TEXTURE_WIDTH + x];
}
