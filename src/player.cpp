#include "player.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>  
#include <ctime>
using namespace std;


/**
 * This function initializes a Player object with default values and loads an image for the player.
 * 
 * @param renderer1 SDL_Renderer pointer used to render graphics on the screen.
 * @param matrix1 The parameter `matrix1` is an integer matrix of size `guia x guia` that represents
 * the game board or maze where the player will move. It is used to initialize the `matrix` data member
 * of the `Player` class.
 * 
 * @return Nothing is being returned in this constructor function.
 */
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


/**
 * The function "desactivar" in the "Player" class sets the boolean variable "poderA" to false after a
 * 5 second delay.
 */
void Player::desactivar() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    poderA=false;
}

/**
 * This function returns the level of the player.
 * 
 * @return The function `getnivel()` is returning the value of the private member variable `nivel` of
 * the `Player` class.
 */
int Player::getnivel(){
    return nivel;
}

/**
 * This function handles keyboard events to move the player and perform certain actions.
 * 
 * @param event The SDL_Event object that contains information about the event that occurred, such as
 * the type of event (e.g. key press, mouse movement, etc.) and any associated data (e.g. which key was
 * pressed).
 */
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

/**
 * This function updates the position of the player in a game, based on the direction they are moving
 * and the obstacles in their path.
 */
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

/**
 * This function renders the game screen, including the player, the game board, and various text
 * elements such as score and level.
 * 
 * @param render2 The SDL_Renderer object used to render the game screen.
 * @param surface2 The surface to be rendered on.
 */
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

/**
 * This function renders the player's texture on the screen using SDL.
 * 
 * @param renderer1 renderer1 is a pointer to an SDL_Renderer object, which is responsible for
 * rendering graphics to a window or screen. It is used to render the player's texture onto the screen.
 */
void Player::getTexture(SDL_Renderer* renderer1){
    SDL_RenderCopy(renderer1, texture, NULL, &destRect);
    SDL_RenderPresent(renderer); 

}



/**
 * The function returns the position of a player as an array of two integers.
 * 
 * @return An integer pointer to an array of two integers representing the position of the player.
 */
int* Player::getPos(){
    static int pos[2];
    pos[0]= y/guia2;
    pos[1]= x/guia2;
    return pos;
}

/**
 * This function sets the position of an enemy for a player in a 2D array.
 * 
 * @param i The index of the enemy whose position is being set.
 * @param pos pos is an array of two integers representing the position of an enemy in a 2D space. The
 * first integer (pos[0]) represents the x-coordinate and the second integer (pos[1]) represents the
 * y-coordinate.
 */
void Player::setEnemigosPos(int i, int pos[2]){
    enemigos[i][0]= pos[0];
    enemigos[i][1]= pos[1];
}


/**
 * The function generates a random position on a matrix while ensuring a minimum distance from a set of
 * enemy positions.
 * 
 * @param enemigos enemigos is a 2D array of integers with 4 rows and 2 columns. Each row represents
 * the position of an enemy on a grid, where the first column is the y-coordinate and the second column
 * is the x-coordinate.
 * @param matrix The "matrix" parameter is a 2D array representing a game board or map. The dimensions
 * of the array are "guia" by "guia", and each element of the array represents a cell on the board. The
 * value of each element can be either 0 or 1,
 * 
 * @return a pointer to an array of two integers, which represents the coordinates of a random position
 * on the matrix that is at least a certain distance away from all the enemy positions. If no such
 * position is found after an infinite loop, the function returns a null pointer.
 */
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


/**
 * This function reduces the player's life by one and randomly assigns a new position on the game
 * matrix.
 */
void Player::reducirVida(){
    --vida;
    int* newPos= getRandomPosition(enemigos, matrix);
    x= newPos[1]*guia2;
    y= newPos[0]*guia2;
}

/**
 * The function returns the value of the boolean variable "poderA" for a Player object.
 * 
 * @return The function `getpoderA` is returning the value of the boolean variable `poderA`.
 */
bool Player::getpoderA() {
    return poderA;
}

/**
 * The function returns the value of the boolean variable "poderD" for a Player object.
 * 
 * @return The function `getpoderD` is returning the value of the boolean variable `poderD`.
 */
bool Player::getpoderD() {
    return poderD;
}

/**
 * The function returns a pointer to the "posPoder" variable of the "Player" class in C++.
 * 
 * @return A pointer to an integer array called `posPoder` is being returned.
 */
int* Player::getPoder(){
    return posPoder;
}

/**
 * The function "quitar" sets a boolean variable to false and updates a matrix element.
 */
void Player::quitar(){
    poderD=false;
    matrix[posPoder[0]][posPoder[1]]=2;
}

/**
 * The function "getvida" returns the current value of the "vida" variable for a Player object.
 * 
 * @return The function `getvida()` is returning the value of the private member variable `vida` of the
 * `Player` class.
 */
int Player::getvida(){
    return vida;
}


/**
 * The function sets the matrix of a player object with the values of a given 2D array.
 * 
 * @param mat mat is a 2D integer array that represents a matrix. The size of the matrix is defined by
 * the constant variable "guia". The function "setmatrix" takes this matrix as a parameter and sets the
 * values of the member variable "matrix" of the class Player to the same values as
 */
void Player::setmatrix(int mat[guia][guia]){
    for (int i = 0; i < guia; i++)
    {
        for (int j = 0; j < guia; j++)
        {
            matrix[i][j]= mat[i][j];
        }
        
    }
}

/**
 * The function resets the position, life, and power status of a player object in a game.
 */
void Player::resetpos(){
    x=0;
    y=0;
    vida=3;
    poderA= false;
    poderD= false;
}

/**
 * The function "puntos" adds 50 points to the player's score.
 */
void Player::puntos(){
    puntaje+=50;
}

/**
 * The function "getpuntos" returns the value of the variable "puntaje" in the class "Player".
 * 
 * @return The function `getpuntos()` is returning the value of the `puntaje` variable, which is the
 * score of the player.
 */
int Player::getpuntos(){
    return puntaje;
}