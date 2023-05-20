#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>

class Socket {
private:
    int server_fd;
    int new_socket;
    int addrlen;
    struct sockaddr_in address;
    // Otros miembros privados

public:
    Socket();
    ~Socket();
    void startListening();
    // Otros métodos públicos
};

#endif
