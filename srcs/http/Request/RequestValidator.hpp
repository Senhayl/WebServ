#pragma once

#include "HttpRequest.hpp"

#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400
#define HTTP_NOT_FOUND 404
#define HTTP_METHOD_NOT_ALLOWED 405
#define HTTP_VERSION_NOT_SUPPORTED 505

class RequestValidator {
public:
	RequestValidator();
	
	bool validate(HttpRequest& request);

private:
	bool isValidMethod(const std::string& method);
	bool isValidPath(const std::string& path);
	bool isValidVersion(const std::string& version);
	bool isValidHost(const std::string& host);
	
	bool isInSet(char c, const std::string& set);
};
