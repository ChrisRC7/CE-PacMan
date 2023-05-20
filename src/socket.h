#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Socket {
public:
    Socket();
    ~Socket();
    void startListening();

private:
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int addrlen;
};

#endif  // SOCKET_H
