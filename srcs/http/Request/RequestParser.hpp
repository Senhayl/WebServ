#pragma once

#include "HttpRequest.hpp"
#include <string>
#include <vector>

class RequestParser {
public:
	RequestParser();
	HttpRequest parse(const std::string& rawRequest);

private:
	std::vector<std::string> splitRequest(const std::string& request);
	
	bool parseRequestLine(const std::string& line, HttpRequest& request);
	bool parseHeader(const std::string& line, HttpRequest& request);

	std::string trim(const std::string& str);
	std::string extractKey(const std::string& s, size_t pos);
	std::string extractValue(const std::string& s, size_t pos);
};
