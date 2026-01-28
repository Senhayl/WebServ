#include "ServerConfig.hpp"

void ServerConfig::setListen(const std::vector<int>& listen){
	_listen = listen;
}

void ServerConfig::addListen(int port){
	_listen.push_back(port);
}

void ServerConfig::setServerName(const std::vector<std::string>& serverName){
	_serverName = serverName;
}

void ServerConfig::addServerName(const std::string& name){
	_serverName.push_back(name);
}

void ServerConfig::setRoot(const std::string& root){
	_root = root;
}

void ServerConfig::setIndex(const std::string& index){
	_index = index;
}

void ServerConfig::setMaxBodySize(const size_t& MBS){
	_clientMaxBodySize = MBS;
}

void ServerConfig::setErrorPage(const std::map<int, std::string>& errorPage){
	_errorPage = errorPage;
}

void ServerConfig::setLocation(const std::vector<Location>& location){
	_locations = location;
}

void ServerConfig::addLocation(const Location& location){
	_locations.push_back(location);
}

const std::vector<int>& ServerConfig::getListen() const {
	return _listen;
}

const std::vector<std::string>& ServerConfig::getServerName() const {
	return _serverName;
}
const std::string& ServerConfig::getRoot() const {
	return _root;
}
const std::string& ServerConfig::getIndex() const {
	return _index;
}
const size_t& ServerConfig::getMBS() const {
	return _clientMaxBodySize;
}
const std::map<int, std::string>& ServerConfig::getErrorPage() const {
	return _errorPage;
}
const std::vector<Location>& ServerConfig::getLocation() const {
	return _locations;
}