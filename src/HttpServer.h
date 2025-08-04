#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>

constexpr int BUFF_SIZE = 8192;

class HttpServer
{
private:
    int port, server_fd;

public:
    HttpServer(int port);
    ~HttpServer();

    void start();
};

#endif
