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

int main(int argc, char **argv)
{
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4221);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        std::cerr << "Failed to bind to port 4221\n";
        return 1;
    }

    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0)
    {
        std::cerr << "listen failed\n";
        return 1;
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
            std::stringstream ss(buffer);
            std::string request, path;
            ss >> request >> path;
            if (path == "/")
            {
                send(client_socket, "HTTP/1.1 200 OK\r\n\r\n", 20, 0);
            }
            else if (path.rfind("/echo/", 0) == 0)
            {
                std::string str = path.substr(6);
                std::string msg = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(str.size()) + "\r\n\r\n" + path.substr(6);
                send(client_socket, msg.c_str(), msg.size(), 0);
            }
            else
            {
                send(client_socket, "HTTP/1.1 404 Not Found\r\n\r\n", 27, 0);
            }
        }

        close(client_socket);
    }

    close(server_fd);

    return 0;
}
