#include "Location.hpp"

// Setters
void Location::setAllowedMethods(const std::vector<std::string>& allowedMethods) {
	_allowedMethods = allowedMethods;
}

void Location::addAllowedMethod(const std::string& method) {
	_allowedMethods.push_back(method);
}

void Location::setPath(const std::string& path) {
	_path = path;
}

void Location::setRoot(const std::string& root) {
	_root = root;
}

void Location::setIndex(const std::string& index) {
	_index = index;
}

void Location::setReturn(const std::string& returnValue) {
	_return = returnValue;
}

void Location::setUploadPath(const std::string& uploadPath) {
	_uploadPath = uploadPath;
}

void Location::setCgiExtensions(const std::string& cgiExtensions) {
	_cgiExtensions = cgiExtensions;
}

void Location::setCgiPath(const std::string& cgiPath) {
	_cgiPath = cgiPath;
}

void Location::setClientMaxBodySize(size_t size) {
	_clientMaxBodySize = size;
}

void Location::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

// Getters
const std::vector<std::string>& Location::getAllowedMethods() const {
	return _allowedMethods;
}

const std::string& Location::getPath() const {
	return _path;
}

const std::string& Location::getRoot() const {
	return _root;
}

const std::string& Location::getIndex() const {
	return _index;
}

const std::string& Location::getReturn() const {
	return _return;
}

const std::string& Location::getUploadPath() const {
	return _uploadPath;
}

const std::string& Location::getCgiExtensions() const {
	return _cgiExtensions;
}

const std::string& Location::getCgiPath() const {
	return _cgiPath;
}

size_t Location::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

bool Location::getAutoindex() const {
	return _autoindex;
}