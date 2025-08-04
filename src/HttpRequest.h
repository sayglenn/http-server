#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <unordered_map>

class HttpRequest
{
private:
    std::string method, path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    HttpRequest(std::string &method, std::string &path, std::unordered_map<std::string, std::string> &headers, std::string &body);

public:
    static HttpRequest parse(const std::string &input);

    const std::string &getMethod() const;
    const std::string &getPath() const;
    const std::unordered_map<std::string, std::string> &getHeaders() const;
    const std::string &getBody() const;
};

#endif
