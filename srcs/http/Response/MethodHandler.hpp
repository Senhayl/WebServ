#pragma once

#include <iostream>
#include "../Request/HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "../../config/ServerConfig.hpp"
#include "../../config/Location.hpp"

class MethodHandler {
public:
	static HttpResponse handlerGET(const HttpRequest& req, const ServerConfig& server, const Location& loc);
	static HttpResponse handlerPOST(const HttpRequest& req, const ServerConfig& server, const Location& loc);
	static HttpResponse handlerDELETE(const HttpRequest& req, const ServerConfig& server, const Location& loc);
};

std::string getContentType(const std::string& path);