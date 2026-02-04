#include "RequestParser.hpp"
#include <sstream>

RequestParser::RequestParser() {}

std::vector<std::string> RequestParser::splitRequest(const std::string& request) {
	std::string buffer;
	std::vector<std::string> arr;
	
	for (size_t i = 0; i < request.length(); i++) {
		if (request[i] == '\n' || i + 1 == request.length()) {
			if ((i > 1 && request[i - 1] == '\r' && request[i - 2] == '\n') || i + 1 == request.length()) {
				buffer.push_back(request[i]);
			} else if (buffer.find('\r') != std::string::npos) {
				buffer.erase(buffer.end() - 1);
			}
			arr.push_back(buffer);
			buffer.clear();
		} else {
			buffer.push_back(request[i]);
		}
	}
	return arr;
}

std::string RequestParser::trim(const std::string& str) {
	size_t start = 0;
	size_t end = str.length();
	
	while (start < end && isspace(str[start])) {
		start++;
	}
	while (end > start && isspace(str[end - 1])) {
		end--;
	}
	return str.substr(start, end - start);
}

std::string RequestParser::extractKey(const std::string& s, size_t pos) {
	return trim(s.substr(0, pos));
}

std::string RequestParser::extractValue(const std::string& s, size_t pos) {
	if (pos + 1 >= s.length()) {
		return "";
	}
	return trim(s.substr(pos + 1));
}

bool RequestParser::parseRequestLine(const std::string& line, HttpRequest& request) {
	std::istringstream iss(line); 
	std::string method, path, version;
	
	if (!(iss >> method >> path >> version)) { // check si tout les mots de la premiere ligne sont presents;
		return false;
	}
	request.setMethod(method);
	request.setPath(path);
	request.setVersion(version);
	
	return true;
}

bool RequestParser::parseHeader(const std::string& line, HttpRequest& request) {
	size_t pos = line.find(':');
	if (pos == std::string::npos) {
		return false;
	}
	std::string key = extractKey(line, pos);
	std::string value = extractValue(line, pos);
	if (key.empty()) {
		return false;
	}
	request.addHeader(key, value);
	return true;
}

HttpRequest RequestParser::parse(const std::string& rawRequest) {
	HttpRequest request;
	std::vector<std::string> lines = splitRequest(rawRequest);
	
	if (lines.empty()) {
		request.setValid(false);
		request.setStatusCode(400);
		return request;
	}
	
	if (!parseRequestLine(lines[0], request)) {
		request.setValid(false);
		request.setStatusCode(400);
		return request;
	}
	
	bool isBody = false;
	std::string bodyContent;
	
	for (size_t i = 1; i < lines.size(); i++) {
		if (lines[i] == "\r\n" || lines[i].empty()) {
			isBody = true;
			continue;
		}
		
		if (isBody) {
			if (!bodyContent.empty()) {
				bodyContent += "\n";
			}
			bodyContent += lines[i];
		} else {
			if (!parseHeader(lines[i], request)) {
				std::cerr << "Warning: Invalid header format: " << lines[i] << std::endl;
			}
		}
	}
	
	if (!bodyContent.empty()) {
		request.setBody(bodyContent);
	}
	request.setValid(true);
	request.setStatusCode(200);
	
	return request;
}
