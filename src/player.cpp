#include "player.h"
#include <iostream>

Player::Player(SDL_Renderer* renderer1) {
    // Cargar la imagen del jugador
    x= 0;
    y= 0;
    SDL_Surface* surface = IMG_Load("img/pac1.png");
    if (surface == nullptr) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    renderer= renderer1;
    texture = SDL_CreateTextureFromSurface(renderer1, surface);
    SDL_Rect rect = { x, y, 24, 24 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    printf("pr");
    SDL_RenderPresent(renderer);
    //SDL_FreeSurface(surface);
}

Player::~Player() {
    //SDL_DestroyTexture(texture);
}

void Player::handleEvent(SDL_Event& event) {
    // Manejar eventos de teclado para mover al jugador
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
            printf("w");
                y -= 10;
                break;
            case SDLK_s:
                y += 10;
                break;
            case SDLK_a:
                x -= 10;
                break;
            case SDLK_d:
                x += 10;
                break;
        }
    }
}

void Player::move() {
    // Actualizar la posición del jugador
}

void Player::render() {
    // Renderizar la textura del jugador en la posición actual
    std::cout<<surface->w<<std::endl;
    SDL_Rect rect = { x, y, 24, 24 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer); 
}

void Player::getTexture(SDL_Renderer* renderer1){
    SDL_RenderCopy(renderer1, texture, NULL, &destRect);
    SDL_RenderPresent(renderer); 

}