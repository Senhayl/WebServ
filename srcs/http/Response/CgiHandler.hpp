#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../Request/HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "../../config/ServerConfig.hpp"
#include "../../config/Location.hpp"

class CgiHandler {
public:
	static HttpResponse execute(const HttpRequest& req, const ServerConfig& server, const Location& loc);

private:
	static std::vector<std::string> buildEnv(const HttpRequest& req, const std::string& scriptPath, const std::string& queryString);
	static std::string parseQueryString(const std::string& path);
	static std::string stripQueryString(const std::string& path);
};
