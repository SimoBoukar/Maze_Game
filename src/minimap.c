#include "../headers/header.h"

#define MINIMAP_SCALE 4  // Scale factor for the minimap

void renderMinimap(SDL_Renderer* renderer) {
  int minimapSize = mapWidth * MINIMAP_SCALE;

  // Draw the background of the minimap
  SDL_Rect minimapRect = {10, 10, minimapSize, minimapSize};
  SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
  SDL_RenderFillRect(renderer, &minimapRect);

  // Draw the walls
  for (int x = 0; x < mapWidth; x++) {
    for (int y = 0; y < mapHeight; y++) {
      if (worldMap[x][y] > 0) {
        SDL_Rect wallRect = {
          10 + x * MINIMAP_SCALE,
          10 + y * MINIMAP_SCALE,
          MINIMAP_SCALE,
          MINIMAP_SCALE
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &wallRect);
      }
    }
  }

  // Draw the player
  SDL_Rect playerRect = {
    10 + (int)(posX * MINIMAP_SCALE) - 2,
    10 + (int)(posY * MINIMAP_SCALE) - 2,
    4,
    4
  };
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &playerRect);

  // Draw the player's direction
  int dirEndX = 10 + (int)((posX + dirX) * MINIMAP_SCALE);
  int dirEndY = 10 + (int)((posY + dirY) * MINIMAP_SCALE);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderDrawLine(renderer,
      10 + (int)(posX * MINIMAP_SCALE),
      10 + (int)(posY * MINIMAP_SCALE),
      dirEndX,
      dirEndY
      );
}
