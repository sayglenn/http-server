#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <string>

#include "HttpRequest.h"

class RequestHandler
{
public:
    const static std::string handleRequest(const HttpRequest &request);
};

#endif