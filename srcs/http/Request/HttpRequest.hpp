#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>


class HttpRequest {
public:
	HttpRequest();
	
	const std::string& getMethod() const {
		return _method;
	}
	const std::string& getPath() const {
		return _path;
	}
	const std::string& getVersion() const {
		return _version;
	}
	const std::string& getBody() const {
		return _body;
	}
	const std::map<std::string, std::string>& getHeaders() const {
		return _headers;
	}
	
	std::string getHeader(const std::string& key) const;
	bool hasHeader(const std::string& key) const;
	
	void setMethod(const std::string& method) {
		_method = method;
	}
	void setPath(const std::string& path) {
		_path = path;
	}
	void setVersion(const std::string& version) {
		_version = version;
	}
	void setBody(const std::string& body) {
		_body = body;
	}
	void addHeader(const std::string& key, const std::string& value);
	
	bool isValid() const {
		return _isValid;
	}
	void setValid(bool valid) {
		_isValid = valid;
	}
	int getStatusCode() const {
		return _statusCode;
	}
	void setStatusCode(int code) {
		_statusCode = code;
	}
	
	void print() const;

private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::map<std::string, std::string> _headers;
	std::string _body;
	bool _isValid;
	int _statusCode;
};
