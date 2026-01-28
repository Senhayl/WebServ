#pragma once 

#include <vector>
#include "Location.hpp"
#include "ServerConfig.hpp"

class Config 
{
	private:
		std::vector<ServerConfig> _servers;

	public:
		void addServer(const ServerConfig& server);

		const std::vector<ServerConfig>& getServers() const;
		const ServerConfig& getServer(size_t index) const;
		size_t getServerCount() const;
} ;