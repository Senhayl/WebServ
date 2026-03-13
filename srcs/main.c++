/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c++                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouron <mlouron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:37:46 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/13 13:53:20 by mlouron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include "server/Server.hpp"
#include "server/Loop.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <set>

int main(int ac, char **av)
{
	if (ac > 2)
	{
		std::cerr << "Usage: " << av[0] << " [config_file]" << std::endl;
		return 1;
	}
	try
	{
		std::string configPath = "./srcs/config/default.conf";
		if (ac == 2)
			configPath = av[1];

		ConfigParser parser;
		Config config = parser.parse(configPath);
		std::vector<std::string> errs;

		const std::vector<ServerConfig>& servers = config.getServers();
		for (size_t i = 0; i < servers.size(); ++i)
			servers[i].isDataValid(errs);

		if (!errs.empty())
		{
			std::cerr << "Config validation errors (" << errs.size() << "):" << std::endl;
			for (size_t i = 0; i < errs.size(); ++i)
				std::cerr << "- " << errs[i] << std::endl;
			return 1;
		}
		if (servers.empty())
		{
			std::cerr << "No server configured" << std::endl;
			return 1;
		}

		std::set<int> uniquePorts;
		for (size_t i = 0; i < servers.size(); ++i)
		{
			const std::vector<int>& listenPorts = servers[i].getListen();
			for (size_t j = 0; j < listenPorts.size(); ++j)
				uniquePorts.insert(listenPorts[j]);
		}

		if (uniquePorts.empty())
		{
			std::cerr << "No listen port configured" << std::endl;
			return 1;
		}

		std::vector<Server*> runtimeServers;
		for (std::set<int>::const_iterator it = uniquePorts.begin(); it != uniquePorts.end(); ++it)
			runtimeServers.push_back(new Server(*it));

		std::cout << "Starting " << runtimeServers.size() << " listening socket(s)..." << std::endl;

		Loop loop(runtimeServers, servers);
		loop.run();

		for (size_t i = 0; i < runtimeServers.size(); ++i)
			delete runtimeServers[i];
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}