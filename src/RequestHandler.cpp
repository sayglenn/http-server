#include "HttpRequest.h"
#include "RequestHandler.h"

const std::string RequestHandler::handleRequest(const HttpRequest &request)
{
    const std::string &path = request.getPath();
    if (path == "/")
    {
        return "HTTP/1.1 200 OK\r\n\r\n";
    }
    else if (path.rfind("/echo/", 0) == 0)
    {
        std::string toEcho = path.substr(6);
        std::string msg = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(toEcho.size()) + "\r\n\r\n" + path.substr(6);
        return msg;
    }
    else if (path == "/user-agent")
    {
        const std::string userAgent = request.getHeaders().at("User-Agent:");
        std::string msg = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(userAgent.size()) + "\r\n\r\n" + userAgent;
        return msg;
    }
    else
    {
        return "HTTP/1.1 404 Not Found\r\n\r\n";
    }
}