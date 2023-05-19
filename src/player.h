#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <thread>

const int guia= 10;
const int guia2= 63;
class Player {
public:
    Player(SDL_Renderer* renderer, int matrix[guia][guia]);
    ~Player();
    void handleEvent(SDL_Event& e);
    void move();
    void render(SDL_Renderer* render, SDL_Surface* surface);
    void getTexture(SDL_Renderer* renderer1);
    void setEnemigosPos(int i, int posE[2]);
    int* getPos();
    void reducirVida();
    bool getpoderD();
    bool getpoderA();
    int* getPoder();
    void desactivar();
    int getnivel();



private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Rect destRect;
    int x;
    int y;
    int matrix[guia][guia];
    int vida;
    int puntaje;
    int enemigos[4][2];
    int posPoder[2];
    bool poderD;
    bool poderA;
    int nivel;
    int poderx;
    int podery;
    TTF_Font* font;
    SDL_Color color;
    std::string dirrection;
    
};

#endif
