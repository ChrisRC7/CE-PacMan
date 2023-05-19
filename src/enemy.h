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

// Estructura para representar un nodo en el mapa
struct Node {
    int x, y;   // Coordenadas del nodo
    int g, h, f; // Valores de los costos para A*
    Node* parent; // Puntero al nodo padre para reconstruir el camino

    Node(int _x, int _y) {
        x = _x;
        y = _y;
        g = h = f = 0;
        parent = NULL;
    }
};
class Enemy {
public:
    Enemy(SDL_Renderer* renderer, int matrix[guia][guia], const char* img, Player* player, int numEnemigo);
    ~Enemy();
    void handleEvent(SDL_Event& e);
    void move(int rut[2]);
    void render();
    void getTexture(SDL_Renderer* renderer1);
    void setPos(int x, int y);
    void newPos();
    void moveback();
    void moveast();



private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Rect destRect;
    int x;
    int y;
    int numEnemigo;
    int matrix[guia][guia];
    std::string dirrection;
    Player* player;
    vector<pair<int, int>> back;
    std::vector<Node*> ast;
    
};

#endif
