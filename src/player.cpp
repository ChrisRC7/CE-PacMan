#include "player.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>  
#include <ctime>
using namespace std;


Player::Player(SDL_Renderer* renderer1, int matrix1[guia][guia]) {
    // Cargar la imagen del jugador
    x= 0;
    y= 0;
    vida= 3;
    puntaje= 0;
    nivel=1;
    poderD=false;
    poderA=false;
    SDL_Surface* surface = IMG_Load("img/pac1.png");
    if (surface == nullptr) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    dirrection= "down";
    for (int i = 0; i < guia; i++)
    {
        for (int j = 0; j < guia; j++)
        {
            matrix[i][j]= matrix1[i][j];
        }
        
    }
    // Inicializar SDL_ttf
    if (TTF_Init() != 0) {
        SDL_Log("Error al inicializar SDL_ttf: %s", TTF_GetError());
    }
    // Cargar una fuente
    font = TTF_OpenFont("/usr/share/fonts/truetype/tlwg/TlwgTypo-Bold.ttf", 18);
    if (font == NULL) {
        SDL_Log("Error al cargar la fuente: %s", TTF_GetError());
    }
    color = {255, 255, 255}; // Color del texto (blanco)
    
    renderer= renderer1;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { x, y, guia2, guia2};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    //SDL_FreeSurface(surface);
}

Player::~Player() {
    //SDL_DestroyTexture(texture);
}


void Player::desactivar() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    poderA=false;
}

int Player::getnivel(){
    return nivel;
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
            case SDLK_p:
                cout<<x/guia2<<endl;
                cout<<y/guia2<<endl;
                break;
            case SDLK_g:
                for (int i = 0; i < guia; i++) {
                      for (int j = 0; j < guia; j++) {
                          cout << "En x: " << i << " En y: " << j << " tiene valor de: " << matrix[i][j] << endl;
                      }
                      }
                break;


            /*case SDLK_r:
                std::cout<<x<<std::endl;
                break;*/
        }
    }
}

void Player::move() {
    // Actualizar la posición del jugador
    if (dirrection == "up"){
        if(matrix[y/guia2 - 1][x/guia2] != 1 && y>0){
            y -= guia2;
            if(matrix[y/guia2][x/guia2]==0){
                puntaje+=10;
                matrix[y/guia2][x/guia2]=2;
            } else if(matrix[y/guia2][x/guia2]==3){
                poderD=false;
                poderA=true;
                std::thread hilo(&Player::desactivar, this);
                hilo.detach(); 
                matrix[y/guia2][x/guia2]=2;
                poderx= x/guia2;
                podery= y/guia2;
            }
        }
    } else if(dirrection == "down"){
        if(matrix[y/guia2 + 1][x/guia2] != 1 && y<567){
            y += guia2;
            if(matrix[y/guia2][x/guia2]==0){
                puntaje+=10;
                matrix[y/guia2][x/guia2]=2;
            } else if(matrix[y/guia2][x/guia2]==3){
                poderD=false;
                poderA=true;
                std::thread hilo(&Player::desactivar, this);
                hilo.detach(); 
                matrix[y/guia2][x/guia2]=2;
                poderx= x/guia2;
                podery= y/guia2;
            }
        }
    } else if(dirrection == "left"){
        if(matrix[y/guia2][x/guia2 - 1]!= 1 && x>0){
            x -= guia2;
            if(matrix[y/guia2][x/guia2]==0){
                puntaje+=10;
                matrix[y/guia2][x/guia2]=2;
            } else if(matrix[y/guia2][x/guia2]==3){
                poderD=false;
                poderA=true;
                std::thread hilo(&Player::desactivar, this);
                hilo.detach(); 
                matrix[y/guia2][x/guia2]=2;
                poderx= x/guia2;
                podery= y/guia2;
            }
        }
    } else{
        if(matrix[y/guia2][x/guia2 + 1] != 1 && x<567){
            x += guia2;
            if(matrix[y/guia2][x/guia2]==0){
                puntaje+=10;
                matrix[y/guia2][x/guia2]=2;
            } else if(matrix[y/guia2][x/guia2]==3){
                poderD=false;
                poderA=true;
                std::thread hilo(&Player::desactivar, this);
                hilo.detach(); 
                matrix[y/guia2][x/guia2]=2;
                poderx= x/guia2;
                podery= y/guia2;
            }
        }
    }
}

void Player::render(SDL_Renderer* render2, SDL_Surface* surface2) {
    Player::move();
    if(poderD==false && puntaje%200==0 && puntaje>199 && poderA==false){
        poderD=true;
        std::srand(std::time(nullptr));
        while(true){
            int y1= std::rand() % 10;
            int x1= std::rand() % 10;
            if(matrix[y1][x1]!= 1){
                cout<<x1<<endl;
                cout<<y1<<endl;
                matrix[y1][x1]= 3;
                posPoder[0]= y1;
                posPoder[1]= x1;
                break;
            }
        }

    }
    int celdasfaltantes=0;
    // Renderizar la textura del jugador en la posición actual
    // Crear un renderizador

    for (int row = 0; row < guia; ++row)
    {
        for (int col = 0; col < guia; ++col)
        {
            
            SDL_Rect cellRect = { col, row, guia, guia};

            if (matrix[row][col] == 1) {
                SDL_FillRect(surface2, &cellRect, SDL_MapRGB(surface2->format, 0, 64, 255));
            }
            else if(matrix[row][col] == 3) {
                SDL_FillRect(surface2, &cellRect, SDL_MapRGB(surface2->format, 102, 255, 102));
            } else if(matrix[row][col] == 0){
                celdasfaltantes++;
                SDL_FillRect(surface2, &cellRect, SDL_MapRGB(surface2->format, 255, 255, 255));
            } else {
                SDL_FillRect(surface2, &cellRect, SDL_MapRGB(surface2->format, 0, 0, 0));
            }
        }
    }

    if(celdasfaltantes==0){
        nivel++;
    }

    char texto1[30];
    char texto2[30];
    char texto3[30];
    snprintf(texto1, sizeof(texto1), "Vida: %d", vida); // Formatear el texto con el valor del entero
    snprintf(texto2, sizeof(texto2), "Puntaje: %d", puntaje); // Formatear el texto con el valor del entero
    snprintf(texto3, sizeof(texto3), "Nivel: %d", nivel); // Formatear el texto con el valor del entero
    SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, texto1, color);
    SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, texto2, color);
    SDL_Surface* textSurface3 = TTF_RenderText_Solid(font, texto3, color);

    // Crear una textura a partir de la superficie de texto
    SDL_Texture* textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
    SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
    SDL_Texture* textTexture3 = SDL_CreateTextureFromSurface(renderer, textSurface3);
    // Posición del texto en la pantalla
    SDL_Rect textRect1 = {640, 0, textSurface1->w, textSurface1->h};
    SDL_Rect textRect2 = {640, 30, textSurface2->w, textSurface2->h};
    SDL_Rect textRect3 = {640, 60, textSurface3->w, textSurface3->h};


    SDL_Rect cellRect = { 10, 0, guia, guia };
    SDL_FillRect(surface2, &cellRect, SDL_MapRGB(surface2->format, 102, 204, 255));
    // Crear la textura a partir de la superficie
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(render2, surface2);
    SDL_RenderCopy(render2, texture1, NULL, NULL);
    SDL_RenderPresent(render2);
    // Renderizar la textura de texto en el renderizador
    SDL_RenderCopy(renderer, textTexture1, NULL, &textRect1);
    SDL_RenderCopy(renderer, textTexture2, NULL, &textRect2);
    SDL_RenderCopy(renderer, textTexture3, NULL, &textRect3);
    SDL_Rect rect = { x, y, guia2, guia2};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer); 
}

void Player::getTexture(SDL_Renderer* renderer1){
    SDL_RenderCopy(renderer1, texture, NULL, &destRect);
    SDL_RenderPresent(renderer); 

}



int* Player::getPos(){
    static int pos[2];
    pos[0]= y/guia2;
    pos[1]= x/guia2;
    return pos;
}

void Player::setEnemigosPos(int i, int pos[2]){
    enemigos[i][0]= pos[0];
    enemigos[i][1]= pos[1];
}


int* getRandomPosition(int enemigos[4][2], int matrix[guia][guia]) {
    const int minima_distancia = 5;
    int row, col;
    int* pos= new int[2];
    std::srand(std::time(nullptr));
    while(true){ 
        int x1 = std::rand() % 10;
        int y1 = std::rand() % 10;
        for (int i = 0; i < 4; i++) {
            int y2 = enemigos[i][0];
            int x2 = enemigos[i][1];
            double distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));    
            if (distancia > minima_distancia && matrix[y1][x1]!=1) {
                pos[0]=y1;
                pos[1]=x1;
                return pos;
            }
        }
    }
    return nullptr;
  
}


void Player::reducirVida(){
    --vida;
    int* newPos= getRandomPosition(enemigos, matrix);
    x= newPos[1]*guia2;
    y= newPos[0]*guia2;
}

bool Player::getpoderA() {
    return poderA;
}

bool Player::getpoderD() {
    return poderD;
}

int* Player::getPoder(){
    return posPoder;
}

void Player::quitar(){
    poderD=false;
    matrix[posPoder[0]][posPoder[1]]=2;
}

int Player::getvida(){
    return vida;
}


void Player::setmatrix(int mat[guia][guia]){
    for (int i = 0; i < guia; i++)
    {
        for (int j = 0; j < guia; j++)
        {
            matrix[i][j]= mat[i][j];
        }
        
    }
}

void Player::resetpos(){
    x=0;
    y=0;
    vida=3;
    poderA= false;
    poderD= false;
}

void Player::puntos(){
    puntaje+=50;
}