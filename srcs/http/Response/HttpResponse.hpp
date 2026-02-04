#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <ctime>
#include <stdlib.h>

class HttpResponse {
public:
	HttpResponse();
	HttpResponse(int statusCode);
	
	int getStatusCode() const {
		return _statusCode;
	}
	void setStatusMessage(int code) {
		switch (code) {
			case 200: _statusMessage = "OK"; break;
			case 201: _statusMessage = "Created"; break;
			case 204: _statusMessage = "No Content"; break;
			case 400: _statusMessage = "Bad Request";break;
			case 404: _statusMessage = "Not Found"; break;
			case 405: _statusMessage = "Method Not Allowed"; break;
			case 500: _statusMessage = "Internal Server Error"; break;
			case 505: _statusMessage = "HTTP Version Not Supported"; break;
			default: _statusMessage = "Unknown"; break;
		}
	}
	const std::string& getBody() const {
		return _body;
	}
	const std::map<std::string, std::string>& getHeaders() const {
		return _headers;
	}

	void setStatusCode(int code);
	void setBody(const std::string& body) {
		_body = body;
		std::ostringstream size;
		size << _body.size();
		this->addHeader("Content-Length", size.str());
	}
	void addHeader(const std::string& key, const std::string& value);
	const std::string getCurrDate();

	std::string toString();
	std::string loadError(int code);
	static HttpResponse createError(int code);
	static HttpResponse createResponse(int code, std::string& body, std::string& contentType);
	
	void print() const;

private:
	int _statusCode;
	std::string _statusMessage;
	std::map<std::string, std::string> _headers;
	std::string _body;
};
