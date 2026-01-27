#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : _statusCode(200), _statusMessage("OK") {}

HttpResponse::HttpResponse(int statusCode) : _statusCode(statusCode), _statusMessage("OK") {}

void HttpResponse::setStatusCode(int code) {
	_statusCode = code;
}

void HttpResponse::addHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

std::string HttpResponse::toString() const {
	std::ostringstream oss;
	
	oss << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin();
		 it != _headers.end(); ++it) {
		oss << it->first << ": " << it->second << "\r\n";
	}
	
	oss << "\r\n";

	if (!_body.empty()) {
		oss << _body;
	}	
	return oss.str();
}

void HttpResponse::print() const {
	std::cout << "=== HTTP Response ===" << std::endl;
	std::cout << "Status: " << _statusCode << " " << _statusMessage << std::endl;
	std::cout << "Headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); 
		 it != _headers.end(); ++it) {
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	}
	if (!_body.empty()) {
		std::cout << "Body Length: " << _body.length() << " bytes" << std::endl;
	}
	std::cout << "===================" << std::endl;
}
