#include "enemy.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;


/**
 * This function creates a new enemy object with a given image, matrix, and player object.
 * 
 * @param renderer1 The SDL_Renderer object used to render graphics in the game.
 * @param matrix1 matrix1 is a 2D integer array that represents the game board or map where the enemy
 * will move. It contains information about the position of walls, obstacles, and other game elements
 * that the enemy needs to avoid or interact with.
 * @param img The parameter "img" is a const char pointer that represents the file path of the image
 * that will be loaded for the enemy object.
 * @param player1 player1 is a pointer to an object of the Player class. It is used to keep track of
 * the player's position and to determine the enemy's behavior.
 * @param numEnem numEnem is an integer variable that represents the number of the enemy being created.
 * It is used to differentiate between multiple enemies in the game.
 * 
 * @return The function does not have a return statement, so nothing is being returned.
 */
Enemy::Enemy(SDL_Renderer* renderer1, int matrix1[guia][guia], const char* img, Player* player1, int numEnem) {
    // Cargar la imagen del jugador
    buscando= false;
    muerto= false;
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
    Enemy::newPos();
}

Enemy::~Enemy() {
    //SDL_DestroyTexture(texture);
}

/**
 * The function moves an enemy object in a game based on a given path and checks for obstacles in the
 * way.
 * 
 * @param rut The parameter "rut" is an integer array of size 2 that represents the target position for
 * the enemy to move towards. The first element of the array (rut[0]) represents the y-coordinate of
 * the target position, and the second element (rut[1]) represents the x-coordinate of the
 */
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
}

/**
 * This function renders the enemy's texture on the screen using SDL.
 * 
 * @param renderer1 renderer1 is a pointer to an SDL_Renderer object, which is responsible for
 * rendering graphics to a window or screen. It is used to render the texture of the enemy object onto
 * the screen.
 */
void Enemy::getTexture(SDL_Renderer* renderer1){
    SDL_RenderCopy(renderer1, texture, NULL, &destRect);
    SDL_RenderPresent(renderer); 

}

/**
 * The function calculates the Manhattan distance between two nodes in a grid.
 * 
 * @param a The parameter "a" is a pointer to a Node object.
 * @param b The parameter "b" in the given code snippet is a pointer to a Node object.
 * 
 * @return The function `heuristic` takes two pointers to `Node` objects as arguments and calculates
 * the Manhattan distance between the two nodes. It returns the sum of the absolute differences between
 * the x-coordinates and y-coordinates of the two nodes.
 */
int heuristic(Node* a, Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

/**
 * The function checks if a vector of nodes contains a specific node based on its x and y coordinates.
 * 
 * @param vec A vector of pointers to Node objects.
 * @param node `node` is a pointer to a `Node` object. It is the node that we want to check if it
 * exists in a vector of `Node` pointers (`vec`). The function `contains_node` returns a boolean value
 * indicating whether the `node` exists in the `vec` vector or
 * 
 * @return The function `contains_node` returns a boolean value indicating whether a given `Node*`
 * object is present in a vector of `Node*` objects or not. It uses the `find_if` algorithm from the
 * `<algorithm>` library to search for the node in the vector based on its `x` and `y` coordinates. If
 * the node is found, the function returns `true`, otherwise
 */
bool contains_node(vector<Node*>& vec, Node* node) {
    return find_if(vec.begin(), vec.end(), [&](Node* n) {
        return n->x == node->x && n->y == node->y;
    }) != vec.end();
}

/**
 * The function returns a pointer to a node in a vector of nodes with a specific x and y coordinate, or
 * NULL if it does not exist.
 * 
 * @param vec a vector of pointers to Node objects
 * @param x The x-coordinate of the node being searched for.
 * @param y The parameter "y" in the function "get_node" is an integer representing the y-coordinate of
 * a node in a two-dimensional grid.
 * 
 * @return The function `get_node` returns a pointer to a `Node` object that has the specified `x` and
 * `y` coordinates if it exists in the `vector<Node*>& vec`. If the node does not exist in the vector,
 * it returns a null pointer.
 */
Node* get_node(vector<Node*>& vec, int x, int y) {
    auto it = find_if(vec.begin(), vec.end(), [&](Node* n) {
        return n->x == x && n->y == y;
    });
    if (it != vec.end()) {
        return *it;
    }
    return NULL;
}

/**
 * The function checks if a given node is present in a vector of nodes representing the open list.
 * 
 * @param open_list A vector of pointers to Node objects representing the open list in an A* search
 * algorithm. The open list contains nodes that have been discovered but not yet explored.
 * @param node The parameter "node" is a pointer to a Node object that we want to check if it is
 * present in the vector "open_list". The function "inOpenList" checks if the given node is present in
 * the open_list vector or not.
 * 
 * @return The function `inOpenList` returns a boolean value. It returns `true` if the input `node` is
 * found in the `open_list` vector, and `false` otherwise.
 */
bool inOpenList(vector<Node*>& open_list, Node* node) {
    for (Node* open_node : open_list) {
        if (open_node == node) {
            return true;
        }
    }
    return false;
}

/**
 * The function checks if a given node is present in a vector of nodes representing a closed list.
 * 
 * @param closed_list A vector of pointers to Node objects representing the closed list in a
 * pathfinding algorithm. The closed list contains nodes that have already been evaluated and expanded,
 * and should not be revisited during the search.
 * @param node The node parameter is a pointer to a Node object that we want to check if it is present
 * in the closed_list vector.
 * 
 * @return a boolean value. It returns true if the given node is found in the closed_list vector, and
 * false otherwise.
 */
bool inClosedList(vector<Node*>& closed_list, Node* node) {
    for (Node* n : closed_list) {
        if (n == node) {
            return true;
        }
    }
    return false;
}

/**
 * The function searches for a node in a list based on its x and y coordinates and returns it if found,
 * otherwise returns NULL.
 * 
 * @param list A vector of pointers to Node objects.
 * @param node The "node" parameter is a pointer to a Node object that we want to find in a vector of
 * Node pointers called "list". The function iterates through the vector and compares the x and y
 * coordinates of each Node object in the vector with the x and y coordinates of the input "node".
 * 
 * @return a pointer to a Node object that matches the x and y coordinates of the input node, if it
 * exists in the input list. If there is no matching node in the list, the function returns a NULL
 * pointer.
 */
Node* getNodeFromList(vector<Node*>& list, Node* node) {
    for (Node* list_node : list) {
        if (list_node->x == node->x && list_node->y == node->y) {
            return list_node;
        }
    }
    return NULL;
}


/**
 * The function prints the x and y coordinates of each node in a vector of Node pointers.
 * 
 * @param list The parameter "list" is a vector of pointers to Node objects.
 */
void printList(vector<Node*> list) {
    for (Node* node : list) {
        cout << "(" << node->x << ", " << node->y << ")" << " -> ";
    }
    cout << endl;
}

/**
 * The function transforms a 2D array into a vector of vectors in C++.
 * 
 * @param matrix The parameter `matrix` is a 2D array of integers with dimensions `guia` x `guia`.
 * 
 * @return The function `transformMatrix` returns a 2D vector of integers
 * (`std::vector<std::vector<int>>`). The vector contains the same elements as the input matrix, which
 * is a 2D array of integers (`int matrix[guia][guia]`). The function copies the elements of the matrix
 * into the vector and returns the vector.
 */
std::vector<std::vector<int>> transformMatrix(int matrix[guia][guia]) {
    std::vector<std::vector<int>> result(guia, std::vector<int>(guia));

    for (int i = 0; i < guia; i++) {
        for (int j = 0; j < guia; j++) {
            result[i][j] = matrix[i][j];
        }
    }

    return result;
}

/**
 * The function implements the A* algorithm to find the shortest path between two points in a matrix.
 * 
 * @param matrix a 2D vector representing the matrix or grid on which the A* algorithm will be applied.
 * Each element of the matrix represents a cell in the grid and can have a value of either 0 or 1,
 * where 0 represents an empty cell and 1 represents a blocked or obstacle cell
 * @param start_x The x-coordinate of the starting point.
 * @param start_y The y-coordinate of the starting point in the matrix.
 * @param end_y The parameter "end_y" represents the y-coordinate of the destination node in the A*
 * algorithm.
 * @param end_x The x-coordinate of the destination node in the matrix.
 * 
 * @return a vector of Node pointers, which represents the path from the starting point to the ending
 * point found by the A* algorithm. If no valid path is found, an empty vector is returned.
 */
vector<Node*> astarP(vector<vector<int>>& matrix, int start_x, int start_y, int end_y, int end_x) {
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

/**
 * The function implements the A* algorithm to find the shortest path between two points in a matrix.
 * 
 * @param matrix a 2D vector representing the matrix or grid on which the A* algorithm will be applied.
 * The matrix contains values that represent the terrain or obstacles on the grid. A value of 1
 * represents a wall or obstacle, while a value of 0 represents a clear path.
 * @param start_x The x-coordinate of the starting point in the matrix.
 * @param start_y The y-coordinate of the starting position in the matrix.
 * @param end_y The parameter "end_y" represents the y-coordinate of the destination node in the
 * matrix/grid.
 * @param end_x The x-coordinate of the destination node.
 * 
 * @return a vector of Node pointers, which represents the path from the starting node to the ending
 * node found using the A* algorithm.
 */
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
    }

    // Si llegamos aquí, no se encontró un camino válido
    return std::vector<Node*>();
}

/**
 * The function backtracking() finds a path between two points in a matrix using a backtracking
 * algorithm.
 * 
 * @param matriz a 2D array representing the grid where the pathfinding algorithm will be applied
 * @param start_row The row index of the starting cell in the matrix.
 * @param start_col Unfortunately, the parameter `start_col` is not provided in the code snippet. Can
 * you please provide more context or the full code so I can assist you better?
 * @param end_row The row coordinate of the end point in the matrix.
 * @param end_col The column index of the end point in the matrix.
 * 
 * @return a vector of pairs of integers, representing the path from the starting cell to the ending
 * cell in the given matrix. If no path is found, an empty vector is returned.
 */
vector<pair<int, int>> backtracking(int matriz[guia][guia], int start_row, int start_col, int end_row, int end_col) {
    // Verificar si las coordenadas de inicio y final son válidas
    if (matriz[start_row][start_col] == 1 && matriz[end_row][end_col] == 1) {
        cout << "Coordenadas de inicio o final inválidas" << endl;
        return {};
    }

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

/**
 * The function takes a vector of Node pointers and two integer coordinates, and returns the position
 * of the first Node in the vector that does not match the given coordinates.
 * 
 * @param list A vector of pointers to Node objects.
 * @param x2 The x-coordinate of the destination position where we want to move the node.
 * @param y2 The parameter `y2` is an integer representing the y-coordinate of a point in a 2D plane.
 * It is used as a reference point to find a node in a list of nodes with matching x and y coordinates.
 * 
 * @return The function `move1` returns either a pointer to the `pos` array (which contains the
 * coordinates of the first node in the `list` that does not have the same coordinates as `(x2, y2)`),
 * or a `nullptr` if all nodes in the `list` have the same coordinates as `(x2, y2)`.
 */
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

/**
 * The function "move2" takes a vector of pairs representing a path, and two integers representing a
 * current position, and returns the next position in the path.
 * 
 * @param camino `camino` is a vector of pairs of integers representing a path or a route. Each pair
 * represents the coordinates of a point in the path. The first integer in the pair represents the
 * y-coordinate and the second integer represents the x-coordinate.
 * @param x2 The x-coordinate of the current position.
 * @param y2 The parameter y2 is an integer representing the y-coordinate of a point.
 * 
 * @return The function `move2` returns a pointer to an array of integers. The array contains the
 * coordinates of the next position in the given `camino` vector that is not equal to the input `x2`
 * and `y2`. If there is no such position, it returns a null pointer.
 */
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



/**
 * The function moves the enemy back to its previous position.
 */
void Enemy::moveback(){
    int* pos2= move2(back,  x/guia2, y/guia2);
    if (pos2!=nullptr){
        Enemy::move(pos2);
        back.erase(back.begin());
    }
}

/**
 * The function moves an enemy object to the east and removes the first element from a vector called
 * "ast".
 */
void Enemy::moveast(){
    int *pos2= move1(ast, x/guia2, y/guia2);
    if (pos2!=nullptr){
        Enemy::move(pos2);
        ast.erase(ast.begin());
    }
}


/**
 * The function generates a new position for an enemy on a game board, ensuring it is a minimum
 * distance away from the player and not on a blocked tile.
 * 
 * @return the new position (x,y) of the enemy on the game board.
 */
void Enemy::newPos() {
    const int minima_distancia = 5;
    int row, col;
    int* pos= new int[2];
    int* posp;
    posp= player->getPos();
    std::srand(std::time(nullptr));
    int y2 = posp[0];
    int x2 = posp[1];
    while(true){ 
        int x1 = (std::rand()+numEnemigo) % 10;
        int y1 = (std::rand()+numEnemigo) % 10;
 
        double distancia = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));    
        if (distancia > minima_distancia && matrix[y1][x1]!=1) {
            cout<<matrix[y1][x1]<<endl;
            x=y1*guia2;
            y=x1*guia2;
            cout<<y<<endl;
            cout<<x<<endl;
            return;
        }
        
    }
  
}



/**
 * The function "desactivar" in the "Enemy" class waits for 5 seconds, updates the position, and sets
 * the "muerto" variable to false.
 */
void Enemy::desactivar() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    newPos();
    muerto= false;
}

/**
 * This function renders the enemy on the screen and controls its movement based on the player's
 * position and power-ups.
 * 
 * @param render1 SDL_Renderer pointer used to render graphics on the screen.
 * @param surface1 The parameter `surface1` is not used in the function `Enemy::render()`. It is
 * declared but not referenced anywhere in the code. Therefore, it is not possible to determine its
 * purpose without additional context or information.
 */
void Enemy::render(SDL_Renderer* render1, SDL_Surface* surface1) {
    if(muerto== false){
        // Renderizar la textura del jugador en la posición actual
        //Enemy::move();
        int* pos;
        int* pos2;
        std::vector<std::vector<int>> vector1;
        std::vector<Node *> ruta1;
        std::vector<pair<int, int>> ruta2;
        pos= player->getPos();
        if(pos[0]==y/guia2 && pos[1]==x/guia2){
            if(player->getpoderA()){
                muerto= true;
                player->puntos();
                std::thread hilo(&Enemy::desactivar, this);
                hilo.detach();
            } else{
                player->reducirVida();
            }
            
        }
        if(player->getpoderD()==false){
            buscando=false;
        }
        if(player->getpoderD() && buscando==false){
            buscando=true;
            if(numEnemigo%2==0){
                pos= player->getPoder();
                back= backtracking(matrix, y/guia2, x/guia2, pos[0], pos[1]);
            } else{
                pos= player->getPoder();
                vector1= transformMatrix(matrix);
                ast= astarP(vector1, x/guia2, y/guia2, pos[0], pos[1]);
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
            vector1= transformMatrix(matrix);
            ast= astar(vector1, x/guia2, y/guia2, pos[0], pos[1]);
            Enemy::moveast();
        }
        int posE[2];
        posE[0]= y/guia2;
        posE[1]= x/guia2;
        player->setEnemigosPos(numEnemigo, posE);
        SDL_Rect rect1 = { x, y, guia2, guia2};
        SDL_RenderCopy(render1, texture, NULL, &rect1);
        SDL_RenderPresent(render1);
    }
}


/**
 * The function sets the matrix of an enemy object with the values from a given 2D array.
 * 
 * @param mat mat is a 2D integer array that represents a matrix. The size of the matrix is guia x
 * guia, where guia is a constant defined somewhere in the code. The function Enemy::setmatrix() takes
 * this matrix as a parameter and sets the values of the matrix member variable of
 */
void Enemy::setmatrix(int mat[guia][guia]){
    for (int i = 0; i < guia; i++)
    {
        for (int j = 0; j < guia; j++)
        {
            matrix[i][j]= mat[i][j];
        }
        
    }
}