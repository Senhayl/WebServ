#include "../httpHeader.hpp"
#include <cstdlib>

static std::string getKey(std::string s, size_t pos) {
	std::string tmp;
	for (size_t i = 0; i < pos; i++) {
		tmp.push_back(s[i]);
	}
	return tmp;
}

static std::string getValue(std::string s, size_t pos) {
	std::string tmp;
	size_t i = pos + 1;
	while (isspace(s[i]))
			i++;
	for (; i < s.length(); i++) {
		tmp.push_back(s[i]);
	}
	return tmp;
}

std::map<std::string, std::string> mapHeaders(std::vector<Token> tokens) {
	std::map<std::string, std::string> tmp;
	std::string key, value;
	for (size_t i = 0; i < tokens.size(); i++) {
		if (tokens[i]._type != HEADERS)
			continue;
		size_t pos = tokens[i]._value.find(':');
		if (pos == std::string::npos) {
			throw std::runtime_error("Invalid header syntax");
		}
		key = getKey(tokens[i]._value, pos);
		value = getValue(tokens[i]._value, pos);
		tmp[key] = value;
	}
	return tmp;
}


static bool isInSet(char c, std::string set) {
	for (size_t i = 0; i < set.length(); i++) {
		if (set[i] == c)
			return true;
	}
	return false;
}

static bool isValidPath(std::string path) {
	if (path[0] != '/' || path.find(' ') != std::string::npos) {
		return false; // error 404
	}
	//check si on la page est dispo
	return true;
}

static bool isValidMethod(std::string method) {
	if (method != "GET" && method != "POST" && method != "DELETE")
		return false; // error 405
	return true;
}

static bool isValidVersion(std::string version) {
	if (version != "HTTP/1.1")
		return false; //error 505
	return true;
}

static bool isValidHost(std::string host) {
	std::string set = "'\' /@\r\n\t";
	for (size_t i = 0; i < host.length(); i++) {
		if (isInSet(host[i], set))
			return false; // error 400
	}
	return true;
}

int checkRequest(std::vector<Token> tokens, std::map<std::string, std::string> headers) {
	for (size_t i = 0; i < tokens.size(); i++) {
		if (tokens[i]._type == METHOD) {
			if (!isValidMethod(tokens[i]._value)) {
				std::cerr << "Invalide method error 405\n";
				return MethodNotAllowed;
			}
		}
		else if (tokens[i]._type == PATH) {
			if (!isValidPath(tokens[i]._value)) {
				std::cerr << "Invalid Path error 404\n";
				return NotFound;
			}
		}
		else if (tokens[i]._type == VERSION) {
			if (!isValidVersion(tokens[i]._value)) {
				std::cerr << "Invalid version error 505\n";
				return WrongVersion;
			}
		}
	}
	std::map<std::string, std::string>::iterator it = headers.find("Host");
	if (it != headers.end() && isValidHost(it->second))
		return OK;
	else
		return BadRequest;
	return OK;
}