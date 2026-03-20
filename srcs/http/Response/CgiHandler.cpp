#include "CgiHandler.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>
#include <sstream>

static std::string intToStr(size_t n) {
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

std::string CgiHandler::stripQueryString(const std::string& path) {
	size_t pos = path.find('?');
	if (pos != std::string::npos)
		return path.substr(0, pos);
	return path;
}

std::string CgiHandler::parseQueryString(const std::string& path) {
	size_t pos = path.find('?');
	if (pos != std::string::npos)
		return path.substr(pos + 1);
	return "";
}

std::vector<std::string> CgiHandler::buildEnv(const HttpRequest& req, const std::string& scriptPath, const std::string& queryString) {
	std::vector<std::string> env;

	env.push_back("REQUEST_METHOD=" + req.getMethod());
	env.push_back("QUERY_STRING=" + queryString);
	env.push_back("SCRIPT_FILENAME=" + scriptPath);
	env.push_back("SCRIPT_NAME=" + stripQueryString(req.getPath()));
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("REDIRECT_STATUS=200");
	env.push_back("CONTENT_LENGTH=" + intToStr(req.getBody().size()));

	if (req.hasHeader("Content-Type"))
		env.push_back("CONTENT_TYPE=" + req.getHeader("Content-Type"));
	else
		env.push_back("CONTENT_TYPE=");

	if (req.hasHeader("Host"))
		env.push_back("HTTP_HOST=" + req.getHeader("Host"));

	if (req.hasHeader("Cookie"))
		env.push_back("HTTP_COOKIE=" + req.getHeader("Cookie"));

	return env;
}

HttpResponse CgiHandler::execute(const HttpRequest& req, const ServerConfig& server, const Location& loc) {
	std::string reqPath = stripQueryString(req.getPath());
	std::string queryString = parseQueryString(req.getPath());

	std::string root = loc.getRoot().empty() ? server.getRoot() : loc.getRoot();
	std::string scriptPath = root + reqPath;

	struct stat st;
	if (stat(scriptPath.c_str(), &st) != 0)
		return HttpResponse::createError(404, server);

	std::vector<std::string> envVec = buildEnv(req, scriptPath, queryString);
	std::vector<char*> envp;
	for (size_t i = 0; i < envVec.size(); i++)
		envp.push_back(const_cast<char*>(envVec[i].c_str()));
	envp.push_back(NULL);

	std::string interpreter = loc.getCgiPath();
	std::vector<char*> argv;
	argv.push_back(const_cast<char*>(interpreter.c_str()));
	argv.push_back(const_cast<char*>(scriptPath.c_str()));
	argv.push_back(NULL);

	int pipeIn[2];
	int pipeOut[2];
	if (pipe(pipeIn) < 0 || pipe(pipeOut) < 0)
		return HttpResponse::createError(500, server);

	pid_t pid = fork();
	if (pid < 0) {
		close(pipeIn[0]);  close(pipeIn[1]);
		close(pipeOut[0]); close(pipeOut[1]);
		return HttpResponse::createError(500, server);
	}

	if (pid == 0) {
		close(pipeIn[1]);
		close(pipeOut[0]);
		dup2(pipeIn[0], STDIN_FILENO);
		dup2(pipeOut[1], STDOUT_FILENO);
		close(pipeIn[0]);
		close(pipeOut[1]);
		execve(interpreter.c_str(), &argv[0], &envp[0]);
		std::cerr << "[CGI] execve failed: " << interpreter << std::endl;
		exit(1);
	}
	close(pipeIn[0]);
	close(pipeOut[1]);

	const std::string& body = req.getBody();
	if (!body.empty())
		write(pipeIn[1], body.c_str(), body.size());
	close(pipeIn[1]);

	int status;
	int elapsed = 0;
	bool childDone = false;
	while (elapsed < 5000) {
		int ret = waitpid(pid, &status, WNOHANG);
		if (ret > 0) {
			childDone = true;
			break;
		}
		usleep(10000);
		elapsed += 10;
	}

	if (!childDone) {
		kill(pid, SIGKILL);
		waitpid(pid, &status, 0);
		close(pipeOut[0]);
		return HttpResponse::createError(504, server);
	}

	std::string output;
	char buf[4096];
	ssize_t n;
	while ((n = read(pipeOut[0], buf, sizeof(buf))) > 0)
		output.append(buf, n);
	close(pipeOut[0]);

	size_t sepPos = output.find("\r\n\r\n");
	std::string sep = "\r\n\r\n";
	if (sepPos == std::string::npos) {
		sepPos = output.find("\n\n");
		sep = "\n\n";
	}

	if (sepPos == std::string::npos)
		return HttpResponse::createError(500, server);

	std::string cgiHeaders = output.substr(0, sepPos);
	std::string cgiBody = output.substr(sepPos + sep.size());

	std::string contentType = "text/html";
	int statusCode = 200;
	std::istringstream headerStream(cgiHeaders);
	std::string line;
	while (std::getline(headerStream, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.find("Content-Type:") == 0 || line.find("Content-type:") == 0)
			contentType = line.substr(line.find(':') + 2);
		if (line.find("Status:") == 0)
			statusCode = std::atoi(line.substr(8).c_str());
	}

	return HttpResponse::createResponse(statusCode, cgiBody, contentType);
}
