#include "CgiHandler.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <fcntl.h>

static std::string intToStr(size_t n) {
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

static std::string trim(const std::string& s) {
	size_t start = 0;
	while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
		++start;
	size_t end = s.size();
	while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
		--end;
	return s.substr(start, end - start);
}

static std::string headerToEnvKey(const std::string& key) {
	std::string out = "HTTP_";
	for (size_t i = 0; i < key.size(); ++i) {
		char c = key[i];
		if (c == '-')
			out += '_';
		else
			out += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
	}
	return out;
}

static void parseHostHeader(const HttpRequest& req, std::string& serverName, std::string& serverPort, const ServerConfig& server) {
	serverName = "localhost";
	serverPort = server.getListen().empty() ? "8080" : intToStr(server.getListen()[0]);

	if (!req.hasHeader("Host"))
		return;

	std::string host = req.getHeader("Host");
	size_t colon = host.find(':');
	if (colon != std::string::npos) {
		serverName = host.substr(0, colon);
		serverPort = host.substr(colon + 1);
	} else {
		serverName = host;
	}
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

std::vector<std::string> CgiHandler::buildEnv(const HttpRequest& req, const ServerConfig& server, const std::string& scriptPath, const std::string& queryString) {
	std::vector<std::string> env;
	std::string serverName;
	std::string serverPort;
	parseHostHeader(req, serverName, serverPort, server);
	std::string cleanPath = stripQueryString(req.getPath());

	env.push_back("REQUEST_METHOD=" + req.getMethod());
	env.push_back("QUERY_STRING=" + queryString);
	env.push_back("REQUEST_URI=" + req.getPath());
	env.push_back("PATH_INFO=" + cleanPath);
	env.push_back("SCRIPT_FILENAME=" + scriptPath);
	env.push_back("SCRIPT_NAME=" + cleanPath);
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("SERVER_NAME=" + serverName);
	env.push_back("SERVER_PORT=" + serverPort);
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

	const std::map<std::string, std::string>& headers = req.getHeaders();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		std::string key = it->first;
		if (key == "Content-Type" || key == "Content-Length")
			continue;
		env.push_back(headerToEnvKey(key) + "=" + it->second);
	}

	return env;
}

bool CgiHandler::startProcess(const HttpRequest& req, const ServerConfig& server, const Location& loc, CgiProcess& process, HttpResponse& errorResp) {
	std::string reqPath = stripQueryString(req.getPath());
	std::string queryString = parseQueryString(req.getPath());

	std::string root = loc.getRoot().empty() ? server.getRoot() : loc.getRoot();
	std::string scriptPath = root + reqPath;

	struct stat st;
	if (stat(scriptPath.c_str(), &st) != 0) {
		errorResp = HttpResponse::createError(404, server);
		return false;
	}

	std::vector<std::string> envVec = buildEnv(req, server, scriptPath, queryString);
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
	if (pipe(pipeIn) < 0 || pipe(pipeOut) < 0) {
		errorResp = HttpResponse::createError(500, server);
		return false;
	}

	pid_t pid = fork();
	if (pid < 0) {
		close(pipeIn[0]);  close(pipeIn[1]);
		close(pipeOut[0]); close(pipeOut[1]);
		errorResp = HttpResponse::createError(500, server);
		return false;
	}

	if (pid == 0) {
		close(pipeIn[1]);
		close(pipeOut[0]);
		dup2(pipeIn[0], STDIN_FILENO);
		dup2(pipeOut[1], STDOUT_FILENO);
		close(pipeIn[0]);
		close(pipeOut[1]);
		size_t slashPos = scriptPath.find_last_of('/');
		if (slashPos != std::string::npos) {
			std::string scriptDir = scriptPath.substr(0, slashPos);
			if (!scriptDir.empty())
				chdir(scriptDir.c_str());
		}
		execve(interpreter.c_str(), &argv[0], &envp[0]);
		exit(1);
	}

	close(pipeIn[0]);
	close(pipeOut[1]);
	fcntl(pipeIn[1], F_SETFL, O_NONBLOCK);
	fcntl(pipeOut[0], F_SETFL, O_NONBLOCK);

	process.pid = pid;
	process.stdinFd = pipeIn[1];
	process.stdoutFd = pipeOut[0];
	return true;
}

HttpResponse CgiHandler::buildResponseFromOutput(const std::string& output) {
	size_t sepPos = output.find("\r\n\r\n");
	std::string sep = "\r\n\r\n";
	if (sepPos == std::string::npos) {
		sepPos = output.find("\n\n");
		sep = "\n\n";
	}

	if (sepPos == std::string::npos)
		return HttpResponse::createError(500);

	std::string cgiHeaders = output.substr(0, sepPos);
	std::string cgiBody = output.substr(sepPos + sep.size());

	std::string contentType = "text/html";
	int statusCode = 200;
	std::vector< std::pair<std::string, std::string> > extraHeaders;
	std::istringstream headerStream(cgiHeaders);
	std::string line;
	while (std::getline(headerStream, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			continue;
		if (line.find("HTTP/") == 0) {
			std::istringstream iss(line);
			std::string proto;
			iss >> proto >> statusCode;
			continue;
		}
		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;
		std::string key = trim(line.substr(0, colon));
		std::string value = trim(line.substr(colon + 1));
		if (key == "Content-Type" || key == "Content-type")
			contentType = value;
		else if (key == "Status")
			statusCode = std::atoi(value.c_str());
		else if (!key.empty())
			extraHeaders.push_back(std::make_pair(key, value));
	}

	HttpResponse resp = HttpResponse::createResponse(statusCode, cgiBody, contentType);
	for (size_t i = 0; i < extraHeaders.size(); ++i)
		resp.addHeader(extraHeaders[i].first, extraHeaders[i].second);
	return resp;
}

HttpResponse CgiHandler::execute(const HttpRequest& req, const ServerConfig& server, const Location& loc) {
	CgiProcess proc;
	HttpResponse errorResp;
	if (!startProcess(req, server, loc, proc, errorResp))
		return errorResp;

	const std::string& body = req.getBody();
	if (!body.empty()) {
		size_t totalWritten = 0;
		while (totalWritten < body.size()) {
			ssize_t w = write(proc.stdinFd, body.c_str() + totalWritten, body.size() - totalWritten);
			if (w <= 0)
				break;
			totalWritten += static_cast<size_t>(w);
		}
	}
	close(proc.stdinFd);

	int status;
	int elapsed = 0;
	bool childDone = false;
	while (elapsed < 5000) {
		int ret = waitpid(proc.pid, &status, WNOHANG);
		if (ret > 0) {
			childDone = true;
			break;
		}
		usleep(10000);
		elapsed += 10;
	}

	if (!childDone) {
		kill(proc.pid, SIGKILL);
		waitpid(proc.pid, &status, 0);
		close(proc.stdoutFd);
		return HttpResponse::createError(504);
	}

	std::string output;
	char buf[4096];
	ssize_t n;
	while ((n = read(proc.stdoutFd, buf, sizeof(buf))) > 0)
		output.append(buf, n);
	close(proc.stdoutFd);

	return buildResponseFromOutput(output);
}
