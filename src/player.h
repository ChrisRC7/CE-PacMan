#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Player {
public:
    Player(SDL_Renderer* renderer, int matrix[25][25]);
    ~Player();
    void handleEvent(SDL_Event& e);
    void move();
    void render();
    void getTexture(SDL_Renderer* renderer1);
    int* getPos();



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
    
};

#endif
