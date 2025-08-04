#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>
#include <fstream>
#include <sstream>

#include "HttpRequest.h"

class RequestHandler
{
public:
    const static std::string handleRequest(const HttpRequest &request, const std::string file_dir);
};

#endif