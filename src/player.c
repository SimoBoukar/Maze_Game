#include "../headers/header.h"

void movePlayer(double moveSpeed, double rotSpeed) {
	double newPosX, newPosY;

	// Move forward if no wall in front of you
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]) {
		newPosX = posX + dirX * moveSpeed;
		newPosY = posY + dirY * moveSpeed;
		if(worldMap[(int)newPosX][(int)posY] == 0) posX = newPosX;
		if(worldMap[(int)posX][(int)newPosY] == 0) posY = newPosY;
	}
	// Move backwards if no wall behind you
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
		newPosX = posX - dirX * moveSpeed;
		newPosY = posY - dirY * moveSpeed;
		if(worldMap[(int)newPosX][(int)posY] == 0) posX = newPosX;
		if(worldMap[(int)posX][(int)newPosY] == 0) posY = newPosY;
	}
	// Rotate to the right
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
		// Both camera direction and camera plane must be rotated
		double oldDirX = dirX;
		dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
		dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
		double oldPlaneX = planeX;
		planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
		planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
	}
	// Rotate to the left
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) {
		// Both camera direction and camera plane must be rotated
		double oldDirX = dirX;
		dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
		dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
		double oldPlaneX = planeX;
		planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
		planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
	}
}
