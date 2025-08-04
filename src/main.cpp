#include "HttpServer.h"

int main()
{
    HttpServer server = HttpServer(4221);
    server.start();
}