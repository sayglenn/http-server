#include "HttpServer.h"

int main(int argc, char **argv)
{
    HttpServer server = HttpServer(4221);
    server.start();
}