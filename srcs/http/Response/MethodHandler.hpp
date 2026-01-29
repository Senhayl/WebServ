#pragma once

#include <iostream>
#include "../Request/HttpRequest.hpp"
#include "HttpResponse.hpp"

class MethodHandler {
public:
	static HttpResponse handlerGET(const HttpRequest& req);
	static HttpResponse handlerPOST(const HttpRequest& req);
	static HttpResponse handlerDELETE(const HttpRequest& req);
};

std::string getContentType(const std::string& path);