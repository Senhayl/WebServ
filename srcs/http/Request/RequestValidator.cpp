#include "RequestValidator.hpp"
#include <iostream>

RequestValidator::RequestValidator() {}

bool RequestValidator::isInSet(char c, const std::string& set) {
	for (size_t i = 0; i < set.length(); i++) {
		if (set[i] == c)
			return true;
	}
	return false;
}

bool RequestValidator::isValidMethod(const std::string& method) {
	if (method == "GET" || method == "POST" || method == "DELETE")
		return true;
	return false;
}

bool RequestValidator::isValidPath(const std::string& path) {
	if (path.empty() || path[0] != '/' || path.find(' ') != std::string::npos) {
		return false;
	}
	return true;
}

bool RequestValidator::isValidVersion(const std::string& version) {
	if (version != "HTTP/1.1")
		return false;
	return true;
}

bool RequestValidator::isValidHost(const std::string& host) {
	std::string forbidden = "'\\ /@\r\n\t";
	for (size_t i = 0; i < host.length(); i++) {
		if (isInSet(host[i], forbidden)) {
			return false;
		}
	}
	return true;
}

bool RequestValidator::validate(HttpRequest& request) {
	if (!isValidMethod(request.getMethod())) {
		std::cerr << "Invalid method: " << request.getMethod() << " (error 405)" << std::endl;
		request.setStatusCode(HTTP_METHOD_NOT_ALLOWED);
		request.setValid(false);
		return false;
	}
	if (!isValidPath(request.getPath())) {
		std::cerr << "Invalid path: " << request.getPath() << " (error 404)" << std::endl;
		request.setStatusCode(HTTP_NOT_FOUND);
		request.setValid(false);
		return false;
	}
	if (!isValidVersion(request.getVersion())) {
		std::cerr << "Invalid version: " << request.getVersion() << " (error 505)" << std::endl;
		request.setStatusCode(HTTP_VERSION_NOT_SUPPORTED);
		request.setValid(false);
		return false;
	}
	if (!request.hasHeader("Host")) {
		std::cerr << "Missing Host header (error 400)" << std::endl;
		request.setStatusCode(HTTP_BAD_REQUEST);
		request.setValid(false);
		return false;
	}
	std::string host = request.getHeader("Host");
	if (!isValidHost(host)) {
		std::cerr << "Invalid Host header: " << host << " (error 400)" << std::endl;
		request.setStatusCode(HTTP_BAD_REQUEST);
		request.setValid(false);
		return false;
	}
	request.setStatusCode(HTTP_OK);
	request.setValid(true);
	return true;
}
