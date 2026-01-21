#include "Server.hpp"

void Server::setListen(const std::vector<int>& listen){
	_listen = listen;
}

void Server::addListen(int port){
	_listen.push_back(port);
}

void Server::setServerName(const std::vector<std::string>& serverName){
	_serverName = serverName;
}

void Server::addServerName(const std::string& name){
	_serverName.push_back(name);
}

void Server::setRoot(const std::string& root){
	_root = root;
}

void Server::setIndex(const std::string& index){
	_index = index;
}

void Server::setMaxBodySize(const size_t& MBS){
	_clientMaxBodySize = MBS;
}

void Server::setErrorPage(const std::map<int, std::string>& errorPage){
	_errorPage = errorPage;
}

void Server::setLocation(const std::vector<Location>& location){
	_locations = location;
}

void Server::addLocation(const Location& location){
	_locations.push_back(location);
}

const std::vector<int>& Server::getListen() const {
	return _listen;
}

const std::vector<std::string>& Server::getServerName() const {
	return _serverName;
}
const std::string& Server::getRoot() const {
	return _root;
}
const std::string& Server::getIndex() const {
	return _index;
}
const size_t& Server::getMBS() const {
	return _clientMaxBodySize;
}
const std::map<int, std::string>& Server::getErrorPage() const {
	return _errorPage;
}
const std::vector<Location>& Server::getLocation() const {
	return _locations;
}