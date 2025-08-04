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

#include "ThreadPool.h"

constexpr int BUFF_SIZE = 8192;
constexpr int NUM_THREADS = 10;

class HttpServer
{
private:
    int port, server_fd;
    ThreadPool pool;

public:
    HttpServer(int port);
    ~HttpServer();

    void start(std::string file_dir = "");
};

#endif
