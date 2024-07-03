#include "../headers/header.h"

double posX = 22, posY = 12;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 0.66;

void castRays(SDL_Renderer* renderer) {
	SDL_SetRenderTarget(renderer, NULL);

	for(int x = 0; x < screenWidth; x++) {
		double cameraX = 2 * x / (double)screenWidth - 1;
		double rayDirX = dirX + planeX * cameraX;
		double rayDirY = dirY + planeY * cameraX;

		int mapX = (int)posX;
		int mapY = (int)posY;

		double deltaDistX = fabs(1 / rayDirX);
		double deltaDistY = fabs(1 / rayDirY);

		int stepX = (rayDirX < 0) ? -1 : 1;
		int stepY = (rayDirY < 0) ? -1 : 1;

		double sideDistX = (rayDirX < 0) ? (posX - mapX) * deltaDistX : (mapX + 1.0 - posX) * deltaDistX;
		double sideDistY = (rayDirY < 0) ? (posY - mapY) * deltaDistY : (mapY + 1.0 - posY) * deltaDistY;

		int hit = 0;
		int side;

		while (hit == 0) {
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			} else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			if (worldMap[mapX][mapY] > 0) hit = 1;
		}

		double perpWallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);

		int lineHeight = (int)(screenHeight / perpWallDist);

		int drawStart = -lineHeight / 2 + screenHeight / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + screenHeight / 2;
		if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

		int texNum = worldMap[mapX][mapY] - 1;
		double wallX = (side == 0) ? posY + perpWallDist * rayDirY : posX + perpWallDist * rayDirX;
		wallX -= floor(wallX);

		int texX = (int)(wallX * TEXTURE_WIDTH);
		if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0)) 
			texX = TEXTURE_WIDTH - texX - 1;

		double step = 1.0 * TEXTURE_HEIGHT / lineHeight;
		double texPos = (drawStart - screenHeight / 2 + lineHeight / 2) * step;

		for (int y = drawStart; y < drawEnd; y++) {
			int texY = (int)texPos & (TEXTURE_HEIGHT - 1);
			texPos += step;

			Uint32 color = getPixel(texNum, texX, texY);
			if (side == 1) {  // Darken y-sides
				Uint8 r = (color >> 16) & 0xFF;
				Uint8 g = (color >> 8) & 0xFF;
				Uint8 b = color & 0xFF;
				r = r * 3 / 4;
				g = g * 3 / 4;
				b = b * 3 / 4;
				color = (r << 16) | (g << 8) | b;
			}

			SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 255);
			SDL_RenderDrawPoint(renderer, x, y);
		}

		// Simplified floor and ceiling casting
		double floorXWall, floorYWall;
		if (side == 0 && rayDirX > 0) {
			floorXWall = mapX;
			floorYWall = mapY + wallX;
		} else if (side == 0 && rayDirX < 0) {
			floorXWall = mapX + 1.0;
			floorYWall = mapY + wallX;
		} else if (side == 1 && rayDirY > 0) {
			floorXWall = mapX + wallX;
			floorYWall = mapY;
		} else {
			floorXWall = mapX + wallX;
			floorYWall = mapY + 1.0;
		}

		if (drawEnd < 0) drawEnd = screenHeight;

		for (int y = drawEnd + 1; y < screenHeight; y++) {
			double currentDist = screenHeight / (2.0 * y - screenHeight);
			double weight = currentDist / perpWallDist;

			double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

			int floorTexX = (int)(currentFloorX * TEXTURE_WIDTH) & (TEXTURE_WIDTH - 1);
			int floorTexY = (int)(currentFloorY * TEXTURE_HEIGHT) & (TEXTURE_HEIGHT - 1);

			// Floor
			Uint32 floorColor = getPixel(3, floorTexX, floorTexY);
			SDL_SetRenderDrawColor(renderer, (floorColor >> 16) & 0xFF, (floorColor >> 8) & 0xFF, floorColor & 0xFF, 255);
			SDL_RenderDrawPoint(renderer, x, y);

			// Ceiling
			Uint32 ceilingColor = getPixel(4, floorTexX, floorTexY);
			SDL_SetRenderDrawColor(renderer, (ceilingColor >> 16) & 0xFF, (ceilingColor >> 8) & 0xFF, ceilingColor & 0xFF, 255);
			SDL_RenderDrawPoint(renderer, x, screenHeight - y);
		}
	}
}
