#include "HttpServer.h"

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        HttpServer server = HttpServer(4221);
        server.start();
    }
    else if (argc == 3 && std::string(argv[1]) == "--directory")
    {
        HttpServer server = HttpServer(4221);
        server.start(std::string(argv[2]));
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " --directory <path>\n";
        return 1;
    }
}