#include "Config.hpp"

void Config::addServer(const Server& server){
	_servers.push_back(server);
}

const std::vector<Server>& Config::getServers() const{
	return _servers;
}

const Server& Config::getServer(size_t index) const{
	return _servers[index];
}

size_t Config::getServerCount() const{
	return _servers.size();
}