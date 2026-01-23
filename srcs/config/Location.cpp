#include "Location.hpp"

Location::Location(){
	_clientMaxBodySize = 0;
	_autoindex = false;
}

Location::~Location(){}

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

bool Location::isDataValid() const {
	std::vector<std::string> errs;
	return isDataValid(errs);
}

bool Location::isDataValid(std::vector<std::string>& errs) const{
	if (_path.empty())
		errs.push_back("location: path is empty");
	else if (_path[0] != '/')
		errs.push_back("location: path must start with '/'");
	if (_root.empty())
		errs.push_back("location: root is empty");
	else if (_root[0] != '/')
		errs.push_back("location: root must start with '/'");

	if (_allowedMethods.empty())
		errs.push_back("location: allowed_methods is empty");
	else
	{
		bool hasInvalidMethod = false;
		bool hasPost = false;
		for (size_t i = 0; i < _allowedMethods.size(); i++)
		{
			const std::string& m = _allowedMethods[i];
			if (m == "POST")
				hasPost = true;
			else if (m != "GET" && m != "DELETE")
				hasInvalidMethod = true;
		}
		if (hasInvalidMethod)
			errs.push_back("location: allowed_methods contains an invalid method (allowed: GET, POST, DELETE)");

		if (!_uploadPath.empty() && !hasPost)
			errs.push_back("location: upload_path is set but POST is not allowed");
	}
	if (_clientMaxBodySize > 1000)
		errs.push_back("location: client_max_body_size is too large");

	if ((_cgiExtensions.empty() && !_cgiPath.empty()) || (!_cgiExtensions.empty() && _cgiPath.empty()))
		errs.push_back("location: cgi_extension and cgi_path must be set together");

	return errs.empty();
}