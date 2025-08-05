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

        pool.enqueue([client_socket, file_dir]() {
            char buffer[BUFF_SIZE];
            std::string leftover;
            bool keep_alive = true;
            
            while (keep_alive) {
                ssize_t n = recv(client_socket, buffer, BUFF_SIZE, 0);
                if (n <= 0)
                    break;
                
                leftover.append(buffer, n);
                while (true) {
                    size_t header_end = leftover.find("\r\n\r\n");
                    if (header_end == std::string::npos) {
                        break;
                    }

                    std::string request_to_header = leftover.substr(0, header_end + 4);
                    size_t content_length = 0;
                    auto headers = HttpRequest::parse(request_to_header).getHeaders();
                    if (headers.count("Content-Length"))
                        content_length = std::stoi(headers["Content-Length"]);

                    size_t total_length = header_end + 4 + content_length;
                    if (leftover.size() < total_length)
                        break; 

                    std::string full_request = leftover.substr(0, total_length);
                    leftover = leftover.substr(total_length);

                    HttpRequest request = HttpRequest::parse(full_request);
                    std::string response = RequestHandler::handleRequest(request, file_dir);
                    send(client_socket, response.c_str(), response.size(), 0);
                    
                    if (headers.count("Connection") && headers["Connection"] == "close") {
                        keep_alive = false;
                    }
                }
            }

            close(client_socket);
        });
    }
}