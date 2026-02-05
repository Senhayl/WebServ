#include "HttpResponse.hpp"

HttpResponse::HttpResponse() : _statusCode(200) {}

HttpResponse::HttpResponse(int statusCode) : _statusCode(statusCode) {}

void HttpResponse::setStatusCode(int code) {
	_statusCode = code;
}

void HttpResponse::addHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

const std::string HttpResponse::getCurrDate() {
	char buf[64];
	time_t t = time(NULL);
	struct tm *gmt = gmtime(&t);
	if (!gmt) {
		return "";
	}
	size_t n = strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
	if (n == 0) {
		return "";
	}
	return std::string(buf);
}

std::string HttpResponse::toString() {
	std::ostringstream oss;
	
	oss << "HTTP/1.1 " << _statusCode << " " << _statusMessage << "\r\n";
	 if (_headers.find("Date") == _headers.end())
        addHeader("Date", getCurrDate());
    if (_headers.find("Server") == _headers.end())
        addHeader("Server", "ninho");
    if (_headers.find("Connection") == _headers.end())
	{
		if (_statusCode == 400)
        	addHeader("Connection", "close");
		else
			addHeader("Connection", "keep-alive");
	}

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

std::string HttpResponse::loadError(int code) {
	std::ostringstream path;
	path << "./srcs/http/ErrorPages/" << code << ".html";
	std::ifstream file(path.str().c_str());
	if (file.is_open()) {
		std::ostringstream content;
		content << file.rdbuf();
		file.close();
		return content.str();
	}
	return "";
}

HttpResponse HttpResponse::createError(int code) {
	HttpResponse resp(code);
	resp.setStatusMessage(code);
	resp.setBody(resp.loadError(code));
	resp.addHeader("Date", resp.getCurrDate());
	resp.addHeader("Server", "ninho");
	resp.addHeader("Content-Type", "text/html");
	return resp;
}

HttpResponse HttpResponse::createResponse(int code, std::string& body, std::string& contentType) {
	HttpResponse resp(code);
	resp.setStatusMessage(code);
	resp.setBody(body);
	resp.addHeader("Date", resp.getCurrDate());
	resp.addHeader("Server", "ninho");
	resp.addHeader("Connection", "keep-alive");
	resp.addHeader("Content-Type", contentType);
	return resp;
}