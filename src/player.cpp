#include "player.h"
#include <iostream>

Player::Player(SDL_Renderer* renderer1, int matrix1[25][25]) {
    // Cargar la imagen del jugador
    x= 0;
    y= 0;
    SDL_Surface* surface = IMG_Load("img/pac1.png");
    if (surface == nullptr) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    dirrection= "right";
    for (int i = 0; i < 25; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            matrix[i][j]= matrix1[i][j];
        }
        
    }
    
    renderer= renderer1;
    texture = SDL_CreateTextureFromSurface(renderer1, surface);
    SDL_Rect rect = { x, y, 24, 24 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
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
                dirrection = "up";
                break;
            case SDLK_s:
                dirrection = "down";
                break;
            case SDLK_a:
                dirrection = "left";
                break;
            case SDLK_d:
                dirrection = "right";
                break;

            case SDLK_r:
                std::cout<<x<<std::endl;
                break;
        }
    }
}

void Player::move() {
    // Actualizar la posición del jugador
    if (dirrection == "up"){
        if(matrix[y/25 - 1][x/25] != 1 && y>0){
            y -= 25;
        }
    } else if(dirrection == "down"){
        if(matrix[y/25 + 1][x/25] != 1 && y<600){
            y += 25;
        }
    } else if(dirrection == "left"){
        if(matrix[y/25][x/25 - 1]!= 1 && x>0){
            x -= 25;
        }
    } else{
        if(matrix[y/25][x/25 + 1] != 1 && x<600){
            x += 25;
        }
    }
}

void Player::render() {
    // Renderizar la textura del jugador en la posición actual
    Player::move();
    SDL_Rect rect = { x, y, 24, 24 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer); 
}

void Player::getTexture(SDL_Renderer* renderer1){
    SDL_RenderCopy(renderer1, texture, NULL, &destRect);
    SDL_RenderPresent(renderer); 

}