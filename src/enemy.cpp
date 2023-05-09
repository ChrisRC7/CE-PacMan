#include "enemy.h"
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>


Enemy::Enemy(SDL_Renderer* renderer1, int matrix1[25][25]) {
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

Enemy::~Enemy() {
    //SDL_DestroyTexture(texture);
}

void Enemy::move() {
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

void Enemy::render() {
    // Renderizar la textura del jugador en la posición actual
    Enemy::move();
    SDL_Rect rect = { x, y, 24, 24 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer); 
}

void Enemy::getTexture(SDL_Renderer* renderer1){
    SDL_RenderCopy(renderer1, texture, NULL, &destRect);
    SDL_RenderPresent(renderer); 

}

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

// Función para obtener la distancia euclidiana entre dos puntos
double getDistance(Point a, Point b) {
    return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

// Función de pathfinding
std::vector<Point> pathfinding(int matrix[25][25], int startX, int startY, int endX, int endY) {
    std::priority_queue<std::pair<double, Point>, std::vector<std::pair<double, Point>>, std::greater<std::pair<double, Point>>> pq;
    pq.push({0, Point(startX, startY)});
    std::vector<Point> prev(25 * 25, Point(-1, -1));
    std::vector<double> dist(25 * 25, 1e9);
    dist[startY * 25 + startX] = 0;
    while (!pq.empty()) {
        Point curr = pq.top().second;
        pq.pop();
        if (curr.x == endX && curr.y == endY) {
            break;
        }
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};
        for (int i = 0; i < 4; i++) {
            int nextX = curr.x + dx[i];
            int nextY = curr.y + dy[i];
            if (nextX >= 0 && nextX < 25 && nextY >= 0 && nextY < 25 && matrix[nextY][nextX] != 1) {
                double weight = getDistance(curr, Point(nextX, nextY));
                if (dist[curr.y * 25 + curr.x] + weight < dist[nextY * 25 + nextX]) {
                    dist[nextY * 25 + nextX] = dist[curr.y * 25 + curr.x] + weight;
                    prev[nextY * 25 + nextX] = curr;
                    pq.push({dist[nextY * 25 + nextX], Point(nextX, nextY)});
                }
            }
        }
    }
    std::vector<Point> path;
    Point curr(endX, endY);
    while (curr.x != startX || curr.y != startY) {
        path.push_back(curr);
        curr = prev[curr.y * 25 + curr.x];
    }
    path.push_back(Point(startX, startY));
    std::reverse(path.begin(), path.end());
    return path;
}