#include <sstream>

#include "HttpRequest.h"

HttpRequest::HttpRequest(std::string &method, std::string &path, std::unordered_map<std::string, std::string> &headers, std::string &body) : method(method), path(path), headers(headers), body(body) {}

HttpRequest HttpRequest::parse(const std::string &input)
{
    // Parse request line, i.e. GET /echo HTTP/1.1
    std::string method, path, dummy;
    std::stringstream ss(input);
    ss >> method >> path >> dummy;

    // Parse headers
    std::string header, content;
    std::unordered_map<std::string, std::string> headers;
    while (ss >> header >> content)
    {
        headers[header] = content;
    }

    // Parse body
    std::string body = "";

    HttpRequest request = HttpRequest(method, path, headers, body);

    return request;
}

const std::string &HttpRequest::getMethod() const { return method; }
const std::string &HttpRequest::getPath() const { return path; }
const std::unordered_map<std::string, std::string> &HttpRequest::getHeaders() const { return headers; }
const std::string &HttpRequest::getBody() const { return body; }