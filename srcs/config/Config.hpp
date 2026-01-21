#pragma once 

#include <vector>
#include "Location.hpp"
#include "Server.hpp"

class Config 
{
	private:
		std::vector<Server> _servers;

	public:
		void addServer(const Server& server);

		const std::vector<Server>& getServers() const;
		const Server& getServer(size_t index) const;
		size_t getServerCount() const;
} ;