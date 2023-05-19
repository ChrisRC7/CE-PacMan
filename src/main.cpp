#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <string>
#include <cstdlib>  
#include <ctime>

#include "player.cpp"
#include "enemy.cpp"
using namespace std;


const int rowm= 10;
const int colm= 10;

const int WINDOW_WIDTH = 830;
const int WINDOW_HEIGHT = 630;
const int CELL_SIZE = 10;
int matrix[rowm][colm];

void matrizrandom(){
    std::srand(std::time(nullptr));
    int num1 = 0, num2 = 1;
    int p1=10, p2= 10;
    
    for (int i = 0; i < rowm; i++)
    {
        for (int j = 0; j < colm; j++)
        {
            matrix[i][j]=0;
        }
        
    }
    
    for (int i = 1; i < rowm; i++)
    {
        for (int j = 1; j < colm; j++)
        {
            if (matrix[i+1][j+1]!=1 and matrix[i+1][j-1]!=1 and matrix[i-1][j+1]!=1 and matrix[i-1][j-1]!=1){
                int rand_num = std::rand() % (p1 + p2) + 1; // Generar un número aleatorio entre 1 y la suma de las posibilidades
                if (rand_num <= p1) {
                    matrix[i][j]=1;
                    p2+=3; // Aumentar la posibilidad del otro número
                } else {
                    p1++; // Aumentar la posibilidad del otro número
                }
            }
            
        }
        
    }

    /*for (int i = 0; i < rowm; i++)
    {
        for (int j = 0; j < colm; j++)
        {
            if (matrix[i][j]==0 and (matrix[i+1][j]==1 and matrix[i-1][j]==1)){
                matrix[i][j]=1;
            }
        }
        
    }*/

    for (int i = 0; i < rowm; i++)
    {
        for (int j = 0; j < colm; j++)
        {
            if (matrix[i][j]==0 and (matrix[i+1][j]==1 and matrix[i-1][j]==1 and matrix[i][j+1]==1 and matrix[i][j-1]==1)){
                if (j<colm-1)
                {
                    matrix[i][j+1]=0;
                } else{
                    matrix[i][j-1]=0;
                }
            }
        }
        
    }
}

int main(int argc, char* argv[]) {
    matrizrandom();
    
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }
    
    // Crear una ventana
    SDL_Window* window = SDL_CreateWindow(
        "Mi ventana",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (window == NULL) {
        SDL_Log("Error al crear la ventana: %s", SDL_GetError());
        return 1;
    }
    
    
    
    // Crear un renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, CELL_SIZE+((WINDOW_WIDTH-WINDOW_HEIGHT)/63), CELL_SIZE, 32, 0, 0, 0, 0);
    

    // Crear la textura a partir de la superficie
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    // Esperar hasta que se cierre la ventana
    bool running = true;
    bool marcador1= true;
    bool marcador2= true;
    bool marcador3= true;
    SDL_Event event;
    Player player(renderer, matrix);
    Enemy enemy1(renderer, matrix, "img/red.png", &player, 1);
    Enemy enemy2(renderer, matrix, "img/light.png", &player, 2);
    Enemy enemy3(renderer, matrix, "img/orange.png", &player, 3);
    Enemy enemy4(renderer, matrix, "img/pink.png", &player, 4);
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            player.handleEvent(event);
        }
        if(player.getvida()<=0 or player.getnivel()>=5){
                running=false;
        }
        if(player.getnivel()==1){
            player.render(renderer, surface);
            enemy1.render(renderer, surface);  
        }
        if(player.getnivel()==2 && marcador1){
            marcador1= false;
            matrizrandom();
            player.resetpos();
            player.setmatrix(matrix);
            enemy1.setmatrix(matrix);
            enemy2.setmatrix(matrix);
        }
        if(player.getnivel()==2 and marcador1== false){
            player.render(renderer, surface);
            enemy2.render(renderer, surface);
            enemy1.render(renderer, surface);
        }
        if(player.getnivel()==3 && marcador2){
            marcador2= false;
            matrizrandom();
            player.resetpos();
            player.setmatrix(matrix);
            enemy1.setmatrix(matrix);
            enemy2.setmatrix(matrix);
            enemy3.setmatrix(matrix);
        }
        if(player.getnivel()==3 and marcador2== false){
            player.render(renderer, surface);
            enemy1.render(renderer, surface);
            enemy2.render(renderer, surface);
            enemy3.render(renderer, surface);
        }
        if(player.getnivel()==4 && marcador3){
            marcador3= false;
            matrizrandom();
            player.resetpos();
            player.setmatrix(matrix);
            enemy1.setmatrix(matrix);
            enemy2.setmatrix(matrix);
            enemy3.setmatrix(matrix);
            enemy4.setmatrix(matrix);
        }
        if(player.getnivel()==4 and marcador3== false){
            player.render(renderer, surface);
            enemy1.render(renderer, surface);
            enemy2.render(renderer, surface);
            enemy3.render(renderer, surface);
            enemy4.render(renderer, surface);
        }
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        
        //SDL_DestroyTexture(texture);
    
        // Actualizar la pantalla
        //SDL_RenderPresent(renderer);
        SDL_Delay(175);  
    }
    
    // Liberar recursos y salir
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
