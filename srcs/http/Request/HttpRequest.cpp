#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : _isValid(false), _statusCode(200) {}

std::string HttpRequest::getHeader(const std::string& key) const {
	std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it != _headers.end()) {
		return it->second;
	}
	return "";
}

bool HttpRequest::hasHeader(const std::string& key) const {
	if (_headers.find(key) != _headers.end())
		return true;
	return false;
}

void HttpRequest::addHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

void HttpRequest::print() const {
	std::cout << "=== HTTP Request ===" << std::endl;
	std::cout << "Method: " << _method << std::endl;
	std::cout << "Path: " << _path << std::endl;
	std::cout << "Version: " << _version << std::endl;
	std::cout << "Headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); 
		 it != _headers.end(); ++it) {
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	}
	if (!_body.empty()) {
		std::cout << "Body: " << _body << std::endl;
	}
	std::cout << "Valid: " << (_isValid ? "Yes" : "No") << std::endl;
	std::cout << "Status Code: " << _statusCode << std::endl;
	std::cout << "===================" << std::endl;
}
