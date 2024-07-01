#ifndef HEADER_H
#define HEADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480
#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

extern int worldMap[mapWidth][mapHeight];
extern double posX, posY;
extern double dirX, dirY;
extern double planeX, planeY;

extern void initMap();
void castRays(SDL_Renderer* renderer);
void drawLine(SDL_Renderer* renderer, int x, int y1, int y2, int color);
void movePlayer(double moveSpeed, double rotSpeed);
void renderMinimap(SDL_Renderer* renderer);
extern SDL_Texture* textures[];
void loadTextures(SDL_Renderer* renderer);
void unloadTextures();

#endif
