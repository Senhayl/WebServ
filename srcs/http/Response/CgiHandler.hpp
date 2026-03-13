#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sys/types.h>
#include "../Request/HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "../../config/ServerConfig.hpp"
#include "../../config/Location.hpp"

class CgiHandler {
public:
	struct CgiProcess {
		pid_t pid;
		int stdinFd;
		int stdoutFd;
	};

	static HttpResponse execute(const HttpRequest& req, const ServerConfig& server, const Location& loc);
	static bool startProcess(const HttpRequest& req, const ServerConfig& server, const Location& loc, CgiProcess& process, HttpResponse& errorResp);
	static HttpResponse buildResponseFromOutput(const std::string& output);

private:
	static std::vector<std::string> buildEnv(const HttpRequest& req, const ServerConfig& server, const std::string& scriptPath, const std::string& queryString);
	static std::string parseQueryString(const std::string& path);
	static std::string stripQueryString(const std::string& path);
};
