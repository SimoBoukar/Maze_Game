#include "../headers/header.h"

double posX = 22, posY = 12;
double dirX = -1, dirY = 0;
double planeX = 0, planeY = 0.66;

void castRays(SDL_Renderer* renderer) {
	for(int x = 0; x < screenWidth; x++) {
		//calculate ray position and direction
		double cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
		double rayDirX = dirX + planeX * cameraX;
		double rayDirY = dirY + planeY * cameraX;

		//which box of the map we're in
		int mapX = (int)posX;
		int mapY = (int)posY;

		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;

		//length of ray from one x or y-side to next x or y-side
		double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
		double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
		double perpWallDist;

		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?

		//calculate step and initial sideDist
		if(rayDirX < 0) {
			stepX = -1;
			sideDistX = (posX - mapX) * deltaDistX;
		} else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - posX) * deltaDistX;
		}
		if(rayDirY < 0) {
			stepY = -1;
			sideDistY = (posY - mapY) * deltaDistY;
		} else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - posY) * deltaDistY;
		}

		//perform DDA
		while(hit == 0) {
			//jump to next map square, either in x-direction, or in y-direction
			if(sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			} else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if(worldMap[mapX][mapY] > 0) hit = 1;
		}

		//Calculate distance projected on camera direction
		if(side == 0) perpWallDist = (sideDistX - deltaDistX);
		else          perpWallDist = (sideDistY - deltaDistY);

		//Calculate height of line to draw on screen
		int lineHeight = (int)(screenHeight / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + screenHeight / 2;
		if(drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + screenHeight / 2;
		if(drawEnd >= screenHeight) drawEnd = screenHeight - 1;

		//choose wall color
		int color;
		switch(worldMap[mapX][mapY]) {
			case 1:  color = 255;  break; //red
			case 2:  color = 200;  break; //green
			case 3:  color = 150;  break; //blue
			case 4:  color = 100;  break; //white
			default: color = 50;   break; //yellow
		}

		//give x and y sides different brightness
		if(side == 1) {color = color / 2;}

		//draw the pixels of the stripe as a vertical line
		drawLine(renderer, x, drawStart, drawEnd, color);

		int texNum = worldMap[mapX][mapY] - 1; // Assuming 1-based indexing in worldMap
		double wallX;
		if (side == 0) wallX = posY + perpWallDist * rayDirY;
		else           wallX = posX + perpWallDist * rayDirX;
		wallX -= floor(wallX);

		int texX = (int)(wallX * TEXTURE_WIDTH);
		if(side == 0 && rayDirX > 0) texX = TEXTURE_WIDTH - texX - 1;
		if(side == 1 && rayDirY < 0) texX = TEXTURE_WIDTH - texX - 1;

		// Draw the textured vertical line
		double step = 1.0 * TEXTURE_HEIGHT / lineHeight;
		double texPos = (drawStart - screenHeight / 2 + lineHeight / 2) * step;

		SDL_Rect srcRect = {texX, 0, 1, TEXTURE_HEIGHT};
		SDL_Rect destRect = {x, drawStart, 1, drawEnd - drawStart};

		for(int y = drawStart; y < drawEnd; y++) {
			int texY = (int)texPos & (TEXTURE_HEIGHT - 1);
			texPos += step;
			srcRect.y = texY;
			destRect.y = y;
			destRect.h = 1;
			SDL_RenderCopy(renderer, textures[texNum], &srcRect, &destRect);
		}

		// Floor and ceiling casting
		double floorXWall, floorYWall;

		// 4 different wall directions possible
		if(side == 0 && rayDirX > 0) {
			floorXWall = mapX;
			floorYWall = mapY + wallX;
		} else if(side == 0 && rayDirX < 0) {
			floorXWall = mapX + 1.0;
			floorYWall = mapY + wallX;
		} else if(side == 1 && rayDirY > 0) {
			floorXWall = mapX + wallX;
			floorYWall = mapY;
		} else {
			floorXWall = mapX + wallX;
			floorYWall = mapY + 1.0;
		}

		double distWall, distPlayer, currentDist;

		distWall = perpWallDist;
		distPlayer = 0.0;

		if (drawEnd < 0) drawEnd = screenHeight; // becomes < 0 when the integer overflows

		// draw the floor from drawEnd to the bottom of the screen
		for(int y = drawEnd + 1; y < screenHeight; y++) {
			currentDist = screenHeight / (2.0 * y - screenHeight);

			double weight = (currentDist - distPlayer) / (distWall - distPlayer);

			double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

			int floorTexX, floorTexY;
			floorTexX = (int)(currentFloorX * TEXTURE_WIDTH) % TEXTURE_WIDTH;
			floorTexY = (int)(currentFloorY * TEXTURE_HEIGHT) % TEXTURE_HEIGHT;

			// floor
			SDL_Rect floorSrcRect = {floorTexX, floorTexY, 1, 1};
			SDL_Rect floorDestRect = {x, y, 1, 1};
			SDL_RenderCopy(renderer, textures[5], &floorSrcRect, &floorDestRect);

			// ceilling (symmetrical)
			SDL_Rect ceilingSrcRect = {floorTexX, floorTexY, 1, 1};
			SDL_Rect ceilingDestRect = {x, screenHeight - y, 1, 1};
			SDL_RenderCopy(renderer, textures[6], &ceilingSrcRect, &ceilingDestRect);
		}

	}
}
