#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include "player.h"

class Enemy {
public:
    Enemy(SDL_Renderer* renderer, int matrix[25][25], const char* img, Player* player);
    ~Enemy();
    void handleEvent(SDL_Event& e);
    void move();
    void render();
    void getTexture(SDL_Renderer* renderer1);



private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Rect destRect;
    int x;
    int y;
    int matrix[25][25];
    std::string dirrection;
    Player* player;
    
};

#endif
