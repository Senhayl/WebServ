#include "Config.hpp"

void Config::addServer(const ServerConfig& server){
	_servers.push_back(server);
}

const std::vector<ServerConfig>& Config::getServers() const{
	return _servers;
}

const ServerConfig& Config::getServer(size_t index) const{
	return _servers[index];
}

size_t Config::getServerCount() const{
	return _servers.size();
}