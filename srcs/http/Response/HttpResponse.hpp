#pragma once

#include <iostream>
#include <string>
#include <map>
#include <sstream>

class HttpResponse {
public:
	HttpResponse();
	HttpResponse(int statusCode);
	
	int getStatusCode() const { return _statusCode; }
	const std::string& getStatusMessage() const { return _statusMessage; }
	const std::string& getBody() const { return _body; }
	const std::map<std::string, std::string>& getHeaders() const { return _headers; }

	void setStatusCode(int code);
	void setStatusMessage(const std::string& message) { _statusMessage = message; }
	void setBody(const std::string& body) { _body = body; }
	void addHeader(const std::string& key, const std::string& value);
	
	std::string toString() const;
	
	void print() const;

private:
	int _statusCode;
	std::string _statusMessage;
	std::map<std::string, std::string> _headers;
	std::string _body;
};
