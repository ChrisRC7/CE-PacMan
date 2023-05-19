#include "enemy.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;


Enemy::Enemy(SDL_Renderer* renderer1, int matrix1[guia][guia], const char* img, Player* player1, int numEnem) {
    // Cargar la imagen del jugador
    Enemy::newPos();
    buscando= false;
    numEnemigo= numEnem;
    player= player1;
    SDL_Surface* surface = IMG_Load(img);
    if (surface == nullptr) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    dirrection= "right";
    for (int i = 0; i < guia; i++)
    {
        for (int j = 0; j < guia; j++)
        {
            matrix[i][j]= matrix1[i][j];
        }
        
    }
    
    renderer= renderer1;
    texture = SDL_CreateTextureFromSurface(renderer1, surface);
    SDL_Rect rect = { x, y, guia2, guia2};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    //SDL_FreeSurface(surface);
}

Enemy::~Enemy() {
    //SDL_DestroyTexture(texture);
}

void Enemy::move(int rut[2]) {
    int y1= rut[0];
    int x1= rut[1];
    if (x1>(x/guia2) && matrix[y1][x1] != 1){
        x+=guia2;
    } else if(y1>(y/guia2) && matrix[y1][x1] != 1){
        y+=guia2;
    } else if(x/(guia2>x1) && y1==(y/guia2) && matrix[y1][x1] != 1){
        x-=guia2;
    } else {
        y-=guia2;
    }
    // Actualizar la posición del jugador
    /*if (dirrection == "up"){
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
    }*/
}

void Enemy::getTexture(SDL_Renderer* renderer1){
    SDL_RenderCopy(renderer1, texture, NULL, &destRect);
    SDL_RenderPresent(renderer); 

}



// Función auxiliar para calcular la distancia Manhattan entre dos nodos
int heuristic(Node* a, Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

// Función auxiliar para buscar un nodo en un vector por sus coordenadas
bool contains_node(vector<Node*>& vec, Node* node) {
    return find_if(vec.begin(), vec.end(), [&](Node* n) {
        return n->x == node->x && n->y == node->y;
    }) != vec.end();
}

// Función auxiliar para obtener un nodo de un vector por sus coordenadas
Node* get_node(vector<Node*>& vec, int x, int y) {
    auto it = find_if(vec.begin(), vec.end(), [&](Node* n) {
        return n->x == x && n->y == y;
    });
    if (it != vec.end()) {
        return *it;
    }
    return NULL;
}

bool inOpenList(vector<Node*>& open_list, Node* node) {
    for (Node* open_node : open_list) {
        if (open_node == node) {
            return true;
        }
    }
    return false;
}

// Función para verificar si un nodo está en la lista cerrada
bool inClosedList(vector<Node*>& closed_list, Node* node) {
    for (Node* n : closed_list) {
        if (n == node) {
            return true;
        }
    }
    return false;
}

Node* getNodeFromList(vector<Node*>& list, Node* node) {
    for (Node* list_node : list) {
        if (list_node->x == node->x && list_node->y == node->y) {
            return list_node;
        }
    }
    return NULL;
}


void printList(vector<Node*> list) {
    for (Node* node : list) {
        cout << "(" << node->x << ", " << node->y << ")" << " -> ";
    }
    cout << endl;
}

std::vector<std::vector<int>> transformMatrix(int matrix[guia][guia]) {
    std::vector<std::vector<int>> result(guia, std::vector<int>(guia));

    for (int i = 0; i < guia; i++) {
        for (int j = 0; j < guia; j++) {
            result[i][j] = matrix[i][j];
        }
    }

    return result;
}

// Función principal para el algoritmo de A*
vector<Node*> astar(vector<vector<int>>& matrix, int start_x, int start_y, int end_y, int end_x) {
    // Crear el nodo de inicio y de destino
    Node* start_node = new Node(start_x, start_y);
    Node* end_node = new Node(end_x, end_y);

    // Crear las listas abierta y cerrada
    vector<Node*> open_list;
    vector<Node*> closed_list;

    // Añadir el nodo de inicio a la lista abierta
    open_list.push_back(start_node);

    // Repetir hasta que la lista abierta esté vacía
    int iteration= 0;
    while (!open_list.empty()) {
        // Encontrar el nodo con menor costo f en la lista abierta
        Node* current_node = open_list[0];
        for (int i = 1; i < open_list.size(); i++) {
            if (open_list[i]->f < current_node->f) {
                current_node = open_list[i];
            }
        }

        // Mover el nodo actual de la lista abierta a la cerrada
        open_list.erase(remove(open_list.begin(), open_list.end(), current_node), open_list.end());
        closed_list.push_back(current_node);

        // Si el nodo actual es el nodo de destino, reconstruir el camino
        if (current_node->x == end_node->x && current_node->y == end_node->y) {
            vector<Node*> path;
            Node* node = current_node;
            while (node != NULL) {
                path.push_back(node);
                node = node->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Explorar los vecinos del nodo actual
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                // Ignorar el propio nodo actual y los nodos diagonales
                if ((i == 0 && j == 0) || (i != 0 && j != 0)) {
                    continue;
                }

                int new_x = current_node->x + i;
                int new_y = current_node->y + j;

                // Ignorar los nodos fuera de los límites del mapa
                if (new_x < 0 || new_x >= guia || new_y < 0 || new_y >= guia) {
                    continue;
                }

                // Ignorar los nodos que son muros
                if (matrix[new_y][new_x] == 1) {
                    continue;
                }

                // Crear el nuevo nodo y calcular su valor f
                Node* neighbor = new Node(new_x, new_y);
                neighbor->parent = current_node;
                neighbor->g = current_node->g + 1;
                neighbor->h = heuristic(neighbor, end_node);
                neighbor->f = neighbor->g + neighbor->h;

                // Si el vecino ya está en la lista cerrada, ignorarlo
                if (inClosedList(closed_list, neighbor)) {
                    delete neighbor;
                    continue;
                }

                // Si el vecino ya está en la lista abierta, actualizar su valor f si es menor
                if (inOpenList(open_list, neighbor)) {
                    Node* existing_node = getNodeFromList(open_list, neighbor);
                    if (neighbor->g < existing_node->g) {
                        existing_node->g = neighbor->g;
                        existing_node->f = existing_node->g + existing_node->h;
                        existing_node->parent = neighbor->parent;
                    }
                    delete neighbor;
                    continue;
                }

                // Agregar el vecino a la lista abierta
                open_list.push_back(neighbor);
            }
        }

        // Imprimir los datos de la iteración actual
        std::cout << "Iteración " << iteration << "\n";
        std::cout << "Celda seleccionada: (" << current_node->x << ", " << current_node->y << ")\n";
        std::cout << "Open list:\n";
        printList(open_list);
        std::cout << "Closed list:\n";
        printList(closed_list);

        iteration++;
    }

    // Si llegamos aquí, no se encontró un camino válido
    return std::vector<Node*>();
}

void prueba(int matri[guia][guia]) {
    for (int i = 0; i < guia; i++) {
        for (int j = 0; j < guia; j++) {
            cout << "En x: " << i << " En y: " << j << " tiene valor de: " << matri[i][j] << endl;
        }
    }
}


vector<pair<int, int>> backtracking(int matriz[guia][guia], int start_row, int start_col, int end_row, int end_col) {
    // Verificar si las coordenadas de inicio y final son válidas
    if (matriz[start_row][start_col] == 1 && matriz[end_row][end_col] == 1) {
        cout << "Coordenadas de inicio o final inválidas" << endl;
        return {};
    }
    prueba(matriz);

    // Declarar una pila para rastrear el camino y un conjunto para rastrear las celdas visitadas
    vector<pair<int, int>> camino;
    set<pair<int, int>> visitadas;

    // Agregar la celda de inicio a la pila y al conjunto de visitadas
    camino.push_back(make_pair(start_row, start_col));
    visitadas.insert(make_pair(start_row, start_col));

    // Iterar mientras haya celdas en la pila
    int iteration= 0;
    while (!camino.empty()) {
        // Obtener la celda actual de la pila
        pair<int, int> celda_actual = camino.back();
        int row = celda_actual.first;
        int col = celda_actual.second;

        // Si la celda actual es la celda final, devolver el camino
        if (row == end_row && col == end_col) {
            cout<<matriz<<endl;
            return camino;
        }
            // Imprimir la celda actual y el camino recorrido hasta ese momento
        cout << "Celda actual: (" << row << ", " << col << ")" << endl;
        cout << "Camino recorrido: ";
        for (auto p : camino) {
            cout << "(" << p.first << ", " << p.second << ") ";
        }
        cout << endl;
        std::cout << "Iteración " << iteration << "\n";
        iteration++;

        // Obtener las celdas adyacentes y agregarlas a la pila si aún no han sido visitadas
        if (start_col<end_col && start_row<=end_row){
             if (col < guia-1 && matriz[row][col+1] != 1 && !visitadas.count(make_pair(row, col+1))) {
                cout<<matriz[row][col+1]<<endl;
                camino.push_back(make_pair(row, col+1));
                visitadas.insert(make_pair(row, col+1));
                continue;
            }
            if (row < guia-1 && matriz[row+1][col] != 1 && !visitadas.count(make_pair(row+1, col))) {
                cout<<matriz[row+1][col]<<endl;
                camino.push_back(make_pair(row+1, col));
                visitadas.insert(make_pair(row+1, col));
                continue;
            }
            if (col > 0 && matriz[row][col-1] != 1 && !visitadas.count(make_pair(row, col-1))) {
                cout<<matriz[row][col-1]<<endl;
                camino.push_back(make_pair(row, col-1));
                visitadas.insert(make_pair(row, col-1));
                continue;
            }
            if (row > 0 && matriz[row-1][col] != 1 && !visitadas.count(make_pair(row-1, col))) {
                cout<<matriz[row-1][col]<<endl;
                camino.push_back(make_pair(row-1, col));
                visitadas.insert(make_pair(row-1, col));
                continue;
            }
            // Si no hay celdas adyacentes sin visitar, retroceder al nodo anterior
            camino.pop_back();
        } else if(start_col<end_col && start_row>=end_row){
            if (col < guia-1 && matriz[row][col+1] != 1 && !visitadas.count(make_pair(row, col+1))) {
                cout<<matriz[row][col+1]<<endl;
                camino.push_back(make_pair(row, col+1));
                visitadas.insert(make_pair(row, col+1));
                continue;
            }
            if (row > 0 && matriz[row-1][col] != 1 && !visitadas.count(make_pair(row-1, col))) {
                cout<<matriz[row-1][col]<<endl;
                camino.push_back(make_pair(row-1, col));
                visitadas.insert(make_pair(row-1, col));
                continue;
            }
            if (col > 0 && matriz[row][col-1] != 1 && !visitadas.count(make_pair(row, col-1))) {
                cout<<matriz[row][col-1]<<endl;
                camino.push_back(make_pair(row, col-1));
                visitadas.insert(make_pair(row, col-1));
                continue;
            }
            if (row < guia-1 && matriz[row+1][col] != 1 && !visitadas.count(make_pair(row+1, col))) {
                cout<<matriz[row+1][col]<<endl;
                camino.push_back(make_pair(row+1, col));
                visitadas.insert(make_pair(row+1, col));
                continue;
            }
            // Si no hay celdas adyacentes sin visitar, retroceder al nodo anterior
            camino.pop_back(); 
        }else if(start_col>end_col && start_col<=end_col){
            if (col > 0 && matriz[row][col-1] != 1 && !visitadas.count(make_pair(row, col-1))) {
                    cout<<matriz[row][col-1]<<endl;
                    camino.push_back(make_pair(row, col-1));
                    visitadas.insert(make_pair(row, col-1));
                    continue;
            }
            if (row < guia-1 && matriz[row+1][col] != 1 && !visitadas.count(make_pair(row+1, col))) {
                cout<<matriz[row+1][col]<<endl;
                camino.push_back(make_pair(row+1, col));
                visitadas.insert(make_pair(row+1, col));
                continue;
            }
            if (col < guia-1 && matriz[row][col+1] != 1 && !visitadas.count(make_pair(row, col+1))) {
                cout<<matriz[row][col+1]<<endl;
                camino.push_back(make_pair(row, col+1));
                visitadas.insert(make_pair(row, col+1));
                continue;
            }
            if (row > 0 && matriz[row-1][col] != 1 && !visitadas.count(make_pair(row-1, col))) {
                cout<<matriz[row-1][col]<<endl;
                camino.push_back(make_pair(row-1, col));
                visitadas.insert(make_pair(row-1, col));
                continue;
            }
            // Si no hay celdas adyacentes sin visitar, retroceder al nodo anterior
            camino.pop_back();
        } else if(start_row<=end_row && start_col<end_col){
            if (col > 0 && matriz[row][col-1] != 1 && !visitadas.count(make_pair(row, col-1))) {
                cout<<matriz[row][col-1]<<endl;
                camino.push_back(make_pair(row, col-1));
                visitadas.insert(make_pair(row, col-1));
                continue;
            }
            if (row < guia-1 && matriz[row+1][col] != 1 && !visitadas.count(make_pair(row+1, col))) {
                cout<<matriz[row+1][col]<<endl;
                camino.push_back(make_pair(row+1, col));
                visitadas.insert(make_pair(row+1, col));
                continue;
            }
            if (col < guia-1 && matriz[row][col+1] != 1 && !visitadas.count(make_pair(row, col+1))) {
                cout<<matriz[row][col+1]<<endl;
                camino.push_back(make_pair(row, col+1));
                visitadas.insert(make_pair(row, col+1));
                continue;
            }
            if (row > 0 && matriz[row-1][col] != 1 && !visitadas.count(make_pair(row-1, col))) {
                cout<<matriz[row-1][col]<<endl;
                camino.push_back(make_pair(row-1, col));
                visitadas.insert(make_pair(row-1, col));
                continue;
            }
            // Si no hay celdas adyacentes sin visitar, retroceder al nodo anterior
            camino.pop_back();
        } else if (start_row>end_row){
            if (row > 0 && matriz[row-1][col] != 1 && !visitadas.count(make_pair(row-1, col))) {
                cout<<matriz[row-1][col]<<endl;
                camino.push_back(make_pair(row-1, col));
                visitadas.insert(make_pair(row-1, col));
                continue;
            }
            if (col > 0 && matriz[row][col-1] != 1 && !visitadas.count(make_pair(row, col-1))) {
                cout<<matriz[row][col-1]<<endl;
                camino.push_back(make_pair(row, col-1));
                visitadas.insert(make_pair(row, col-1));
                continue;
            }
            if (row < guia-1 && matriz[row+1][col] != 1 && !visitadas.count(make_pair(row+1, col))) {
                cout<<matriz[row+1][col]<<endl;
                camino.push_back(make_pair(row+1, col));
                visitadas.insert(make_pair(row+1, col));
                continue;
            }
            if (col < guia-1 && matriz[row][col+1] != 1 && !visitadas.count(make_pair(row, col+1))) {
                cout<<matriz[row][col+1]<<endl;
                camino.push_back(make_pair(row, col+1));
                visitadas.insert(make_pair(row, col+1));
                continue;
            }
            // Si no hay celdas adyacentes sin visitar, retroceder al nodo anterior
            camino.pop_back();
        } else if(start_col<=end_col) {
            if (row < guia-1 && matriz[row+1][col] != 1 && !visitadas.count(make_pair(row+1, col))) {
                cout<<matriz[row+1][col]<<endl;
                camino.push_back(make_pair(row+1, col));
                visitadas.insert(make_pair(row+1, col));
                continue;
            }
            if (row > 0 && matriz[row-1][col] != 1 && !visitadas.count(make_pair(row-1, col))) {
                cout<<matriz[row-1][col]<<endl;
                camino.push_back(make_pair(row-1, col));
                visitadas.insert(make_pair(row-1, col));
                continue;
            }
            if (col < guia-1 && matriz[row][col+1] != 1 && !visitadas.count(make_pair(row, col+1))) {
                cout<<matriz[row][col+1]<<endl;
                camino.push_back(make_pair(row, col+1));
                visitadas.insert(make_pair(row, col+1));
                continue;
            }
            if (col > 0 && matriz[row][col-1] != 1 && !visitadas.count(make_pair(row, col-1))) {
                cout<<matriz[row][col-1]<<endl;
                camino.push_back(make_pair(row, col-1));
                visitadas.insert(make_pair(row, col-1));
                continue;
            }
            // Si no hay celdas adyacentes sin visitar, retroceder al nodo anterior
            camino.pop_back();
        } else{
            if (col > 0 && matriz[row][col-1] != 1 && !visitadas.count(make_pair(row, col-1))) {
                cout<<matriz[row][col-1]<<endl;
                camino.push_back(make_pair(row, col-1));
                visitadas.insert(make_pair(row, col-1));
                continue;
            }
            if (row > 0 && matriz[row-1][col] != 1 && !visitadas.count(make_pair(row-1, col))) {
                cout<<matriz[row-1][col]<<endl;
                camino.push_back(make_pair(row-1, col));
                visitadas.insert(make_pair(row-1, col));
                continue;
            }
            if (col < guia-1 && matriz[row][col+1] != 1 && !visitadas.count(make_pair(row, col+1))) {
                cout<<matriz[row][col+1]<<endl;
                camino.push_back(make_pair(row, col+1));
                visitadas.insert(make_pair(row, col+1));
                continue;
            }
            if (row < guia-1 && matriz[row+1][col] != 1 && !visitadas.count(make_pair(row+1, col))) {
                cout<<matriz[row+1][col]<<endl;
                camino.push_back(make_pair(row+1, col));
                visitadas.insert(make_pair(row+1, col));
                continue;
            }
            // Si no hay celdas adyacentes sin visitar, retroceder al nodo anterior
            camino.pop_back();
            }
        }
        

    // Si no se puede encontrar un camino, devolver una lista vacía
    cout << "No se encontró ningún camino" << endl;
    return {};
}

void printList2(vector<pair<int, int>> camino){
    for (auto p : camino) {
    cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << endl;
}

int* move1(vector<Node*> list, int x2, int y2) {
    int x1;
    int y1;
    static int pos[2];
    for (Node* node : list) {
        x1= node->x;
        y1= node->y;
        if(x1!=x2 or y1!=y2) {
            pos[0]= y1;
            pos[1]= x1;
            return pos;
        }
    }
    return nullptr;
}

int* move2(vector<pair<int, int>> camino, int x2, int y2){
    int x1;
    int y1;
    static int pos[2];
    int len= camino.size();
    for (int i = 0; i<len; i++) {
        y1= camino[i].first;
        x1= camino[i].second;
        if(x1!=x2 or y1!=y2) {
            pos[0]= y1;
            pos[1]= x1;
            return pos;
        }
    }
    return nullptr;
}


void Enemy::handleEvent(SDL_Event& event) {
    // Manejar eventos de teclado para mover al jugador
    int* pos;
    int* pos2;
    std::vector<std::vector<int>> vector1;
    std::vector<Node *> ruta1;
    std::vector<pair<int, int>> ruta2;
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_r:
                pos= player->getPos();
                vector1= transformMatrix(matrix);
                ruta1= astar(vector1, x/guia2, y/guia2, pos[0], pos[1]);
                std::cout<<"La ruta es: "<<endl;
                printList(ruta1);
                pos2= move1(ruta1, x/guia2, y/guia2);
                if (pos2!=nullptr){
                    Enemy::move(pos2);
                    std::cout<<pos2[0]<<endl;
                    std::cout<<pos2[1]<<endl;
                }
                break;

            case SDLK_t:
                pos= player->getPos();
                ruta2= backtracking(matrix, y/guia2, x/guia2, pos[0], pos[1]);
                std::cout<<"La ruta es: "<<endl;
                printList2(ruta2);
                pos2= move2(ruta2,  x/guia2, y/guia2);
                if (pos2!=nullptr){
                    Enemy::move(pos2);
                    std::cout<<pos2[0]<<endl;
                    std::cout<<pos2[1]<<endl;
                }
                break;
            case SDLK_h:
                pos= player->getPos();
                cout<<matrix[pos[0]+1][pos[1]]<<endl;
                break;

        }
    }
}

void Enemy::moveback(){
    int* pos2= move2(back,  x/guia2, y/guia2);
    if (pos2!=nullptr){
        Enemy::move(pos2);
        back.erase(back.begin());
    }
}

void Enemy::moveast(){
    int *pos2= move1(ast, x/guia2, y/guia2);
    if (pos2!=nullptr){
        Enemy::move(pos2);
        ast.erase(ast.begin());
    }
}


void Enemy::newPos() {
    const int minima_distancia = 3;
    int row, col;
    int* pos= new int[2];
    int* posp= player->getPos();
    std::srand(std::time(nullptr));
    while(true){ 
        int x1 = std::rand() % 10;
        int y1 = std::rand() % 10;
        for (int i = 0; i < 4; i++) {
            int y2 = posp[0];
            int x2 = posp[1];
            double distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));    
            if (distancia < minima_distancia) {
                x=y1*guia2;
                y=x1*guia2;

            }
        }
    }
  
}


void Enemy::render() {
    // Renderizar la textura del jugador en la posición actual
    //Enemy::move();
    int* pos;
    int* pos2;
    std::vector<std::vector<int>> vector1;
    std::vector<Node *> ruta1;
    std::vector<pair<int, int>> ruta2;
    if(player->getpoderD() && buscando==false){
        buscando=true;
        if(numEnemigo%2==0){
            pos= player->getPoder();
            back= backtracking(matrix, y/guia2, x/guia2, pos[0], pos[1]);
        } else{
            pos= player->getPoder();
            vector1= transformMatrix(matrix);
            ast= astar(vector1, x/guia2, y/guia2, pos[0], pos[1]);
        }
    } else if(buscando== true){
        pos= player->getPoder();
        if(pos[0]==y/guia2 && pos[1]==x/guia2){
            player->quitar();
        }
        if(numEnemigo%2==0){
            Enemy::moveback();
        } else{
            Enemy::moveast();
        }
    } else{
        pos= player->getPos();
        if(pos[0]==y/guia2 && pos[1]==x/guia2){
            player->reducirVida();
        }
        pos= player->getPos();
        vector1= transformMatrix(matrix);
        ast= astar(vector1, x/guia2, y/guia2, pos[0], pos[1]);
        Enemy::moveast();
    }
    int posE[2];
    posE[0]= y/guia2;
    posE[1]= x/guia2;
    player->setEnemigosPos(numEnemigo, posE);
    SDL_Rect rect = { x, y, guia2, guia2};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer); 
}


void Enemy::setmatrix(int mat[guia][guia]){
    for (int i = 0; i < guia; i++)
    {
        for (int j = 0; j < guia; j++)
        {
            matrix[i][j]= mat[i][j];
        }
        
    }
}