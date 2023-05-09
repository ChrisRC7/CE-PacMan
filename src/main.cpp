#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "player.cpp"
using namespace std;


const int rowm= 25;
const int colm= 25;

const int WINDOW_WIDTH = 825;
const int WINDOW_HEIGHT = 625;
const int CELL_SIZE = WINDOW_HEIGHT / rowm;

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));
    int num1 = 0, num2 = 1;
    int p1=10, p2= 10;

    int matrix[rowm][colm];
    
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

    for (int i = 0; i < rowm; i++)
    {
        for (int j = 0; j < colm; j++)
        {
            if (matrix[i][j]==0 and (matrix[i+1][j]==1 and matrix[i-1][j]==1)){
                matrix[i][j]=1;
            }
        }
        
    }

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

    SDL_Surface* surface = SDL_CreateRGBSurface(0, CELL_SIZE+8, CELL_SIZE, 32, 0, 0, 0, 0);
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // establacer el color de fondo en rojo
    //SDL_RenderClear(renderer); // limpiar la pantalla con el color de fondo

    for (int row = 0; row < rowm; ++row)
    {
        for (int col = 0; col < colm; ++col)
        {
            
            SDL_Rect cellRect = { col, row, CELL_SIZE, CELL_SIZE };

            if (matrix[row][col] == 1)
            {
                SDL_FillRect(surface, &cellRect, SDL_MapRGB(surface->format, 0, 64, 255));
            }
            else
            {
                SDL_FillRect(surface, &cellRect, SDL_MapRGB(surface->format, 0, 0, 0));
            }
        }
    }

    SDL_Rect cellRect = { 25, 0, CELL_SIZE, CELL_SIZE };
    SDL_FillRect(surface, &cellRect, SDL_MapRGB(surface->format, 255, 0, 0));
    

    
    if (renderer == NULL) {
        SDL_Log("Error al crear el renderizador: %s", SDL_GetError());
        return 1;
    }
    
    // Crear la textura a partir de la superficie
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);


    // Actualizar la ventana
    //SDL_RenderPresent(renderer);

    
    // Crear un rectángulo SDL para representar la posición de la imagen
    //SDL_Rect destRect = { 0, 0, image->w, image->h };
    
    // Esperar hasta que se cierre la ventana
    bool running = true;
    SDL_Event event;
    Player player(renderer, matrix);
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            player.handleEvent(event);
            
          


        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        player.render();
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
