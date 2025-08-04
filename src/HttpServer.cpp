#include "HttpServer.h"
#include "RequestHandler.h"

HttpServer::HttpServer(int port) : port(port), pool(NUM_THREADS) {}

HttpServer::~HttpServer() { close(server_fd); }

void HttpServer::start(const std::string file_dir)
{
    std::cout << "File directory: " << file_dir << '\n';

    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Failed to create server socket\n";
        return;
    }

    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "setsockopt failed\n";
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port " << port << '\n';
        return;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0)
    {
        std::cerr << "Listen failed\n";
        return;
    }

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    std::cout << "Waiting for a client to connect...\n";

    while (true)
    {
        int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len);
        if (client_socket < 0)
        {
            std::cerr << "Failed to accept connection\n";
            continue;
        }
        std::cout << "Client connected\n";

        char buffer[BUFF_SIZE];
        ssize_t n = recv(client_socket, buffer, BUFF_SIZE - 1, 0);

        if (n < 0)
        {
            std::cerr << "recv failed" << '\n';
        }
        else if (n == 0)
        {
            std::cout << "No data sent" << '\n';
        }
        else
        {
            buffer[n] = '\0';
            std::string str_request(buffer, n);
            pool.enqueue([str_request, client_socket, file_dir]()
                         {
                HttpRequest request = HttpRequest::parse(str_request);
                std::string response = RequestHandler::handleRequest(request, file_dir);
                send(client_socket, response.c_str(), response.size(), 0);
                close(client_socket); });
        }
    }
}