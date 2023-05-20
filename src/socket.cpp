#include "socket.h"

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

Socket::Socket() {
    int opt = 1;
    addrlen = sizeof(address);

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Error al crear socket" << std::endl;
        throw std::runtime_error("Error al crear socket");
    }

    // Opción de socket para reutilizar la dirección
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Error en setsockopt" << std::endl;
        throw std::runtime_error("Error en setsockopt");
    }

    // Configurar dirección del socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5001);

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Error en setsockopt" << std::endl;
        throw std::runtime_error("Error en setsockopt");
    }

    // Enlazar socket a la dirección y puerto especificados
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        std::cerr << "Error en bind" << std::endl;
        throw std::runtime_error("Error en bind");
    }

    // Escuchar conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error en listen" << std::endl;
        throw std::runtime_error("Error en listen");
    }

    // Aceptar nueva conexión
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
        std::cerr << "Error en accept" << std::endl;
        throw std::runtime_error("Error en accept");
    }

    // Conexión aceptada
    std::cout << "Nueva conexión aceptada" << std::endl;
}


Socket::~Socket() {
    // Cerrar la conexión si está abierta
    if (new_socket != -1) {
        close(new_socket);
    }

    // Cerrar el socket del servidor
    if (close(server_fd) == -1) {
        std::cerr << "Error al cerrar el socket: " << std::strerror(errno) << std::endl;
    }
}

void Socket::startListening() {
    // Esperar a que llegue una conexión
    std::cout << "Servidor en espera de conexiones..." << std::endl;

    // Aceptar nueva conexión si no hay una conexión activa
    if (new_socket == -1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            std::cerr << "Error en accept" << std::endl;
            return;
        }

        // Conexión aceptada
        std::cout << "Nueva conexión aceptada" << std::endl;
    }

    // Procesar mensajes entrantes continuamente
    while (true) {
        // Leer mensaje del cliente
        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, 1024);
        std::cout << "valread: " << valread << std::endl;

        if (valread <= 0) {
            std::cout << "Cliente desconectado" << std::endl;
            break;
        }

        std::cout << "Mensaje recibido: " << buffer << std::endl;

        // Enviar respuesta al cliente
        char respuesta[] = "Mensaje recibido.\n";
        send(new_socket, respuesta, sizeof(respuesta), 0);
        std::cout << "Respuesta: " << respuesta << std::endl;
    }

    // Cerrar la conexión y restablecer new_socket a -1
    close(new_socket);
    new_socket = -1;
}