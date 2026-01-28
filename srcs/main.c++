#include "config/ConfigParser.hpp"
#include <iostream>

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return 1;
	}
	try
	{
		ConfigParser parser;
		Config config = parser.parse(av[1]);

		// Afficher tous les serveurs
		const std::vector<ServerConfig>& servers = config.getServers();
		std::cout << "=== Total Servers: " << servers.size() << " ===" << std::endl << std::endl;

		for (size_t i = 0; i < servers.size(); ++i)
		{
			const ServerConfig& server = servers[i];
			std::cout << "--- Server " << i + 1 << " ---" << std::endl;

			// Afficher les ports
			const std::vector<int>& ports = server.getListen();
			std::cout << "Ports: ";
			for (size_t j = 0; j < ports.size(); ++j)
				std::cout << ports[j] << " ";
			std::cout << std::endl;

			// Afficher les noms de serveur
			const std::vector<std::string>& server_names = server.getServerName();
			std::cout << "Server Names: ";
			for (size_t j = 0; j < server_names.size(); ++j)
				std::cout << server_names[j] << " ";
			std::cout << std::endl;

			// Afficher autres données
			std::cout << "Root: " << server.getRoot() << std::endl;
			std::cout << "Index: " << server.getIndex() << std::endl;
			std::cout << "Max Body Size: " << server.getMBS() << std::endl;

			// Afficher les locations
			const std::vector<Location>& locations = server.getLocation();
			std::cout << "Locations: " << locations.size() << std::endl;

			for (size_t j = 0; j < locations.size(); ++j)
			{
				const Location& location = locations[j];
				std::cout << "  Location " << j + 1 << ":" << std::endl;
				std::cout << "    Path: " << location.getPath() << std::endl;

				const std::vector<std::string>& methods = location.getAllowedMethods();
				std::cout << "    Allowed Methods: ";
				for (size_t k = 0; k < methods.size(); ++k)
					std::cout << methods[k] << " ";
				std::cout << std::endl;

				std::cout << "    Root: " << location.getRoot() << std::endl;
				std::cout << "    Index: " << location.getIndex() << std::endl;
				std::cout << "    Upload Path: " << location.getUploadPath() << std::endl;
				std::cout << "    Autoindex: " << (location.getAutoindex() ? "on" : "off") << std::endl;
				std::cout << "    Max Body Size: " << location.getClientMaxBodySize() << std::endl;
			}

			std::cout << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}