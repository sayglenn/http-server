#include <sstream>

#include "HttpRequest.h"

HttpRequest::HttpRequest(std::string &method, std::string &path, std::unordered_map<std::string, std::string> &headers, std::string &body) : method(method), path(path), headers(headers), body(body) {}

HttpRequest HttpRequest::parse(const std::string &input)
{
    // Parse request line, i.e. GET /echo HTTP/1.1
    std::string method, path, version;
    std::stringstream ss(input);
    std::string line;
    getline(ss, line);
    std::stringstream reqLine(line);
    reqLine >> method >> path >> version;

    // Parse headers
    std::string header, content;
    std::unordered_map<std::string, std::string> headers;
    while (std::getline(ss, line) && line != "\r")
    {
        std::stringstream headerLine(line);
        headerLine >> header >> content;
        header.pop_back();
        headers[header] = content;
    }

    // Parse body
    std::string body = "";
    if (headers.count("Content-Length"))
    {
        int body_len = stoi(headers["Content-Length"]);
        body.resize(body_len);
        ss.read(&body[0], body_len);
    }

    return HttpRequest(method, path, headers, body);
}

const std::string &HttpRequest::getMethod() const { return method; }
const std::string &HttpRequest::getPath() const { return path; }
const std::unordered_map<std::string, std::string> &HttpRequest::getHeaders() const { return headers; }
const std::string &HttpRequest::getBody() const { return body; }