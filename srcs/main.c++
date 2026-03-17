/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c++                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:37:46 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/17 14:48:22 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include "server/Server.hpp"
#include "server/Loop.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <csignal>

static void cleanupServers(std::vector<Server*>& listeningServers)
{
	for (size_t i = 0; i < listeningServers.size(); ++i)
		delete listeningServers[i];
	listeningServers.clear();
}

int main(int ac, char **av)
{
	std::vector<Server*> listeningServers;

	if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return 1;
	}
	try
	{
		ConfigParser parser;
		Config config = parser.parse(av[1]);
		std::vector<std::string> errs;
		std::set<int> openedPorts;

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
		std::cout << "Starting " << servers.size() << " server(s)..." << std::endl;

		if (servers.empty())
		{
			std::cerr << "No server configured" << std::endl;
			return 1;
		}

		for (size_t i = 0; i < servers.size(); ++i)
		{
			const std::vector<int>& listens = servers[i].getListen();
			for (size_t j = 0; j < listens.size(); ++j)
			{
				int port = listens[j];
				if (openedPorts.insert(port).second)
					listeningServers.push_back(new Server(port));
			}
		}

		if (listeningServers.empty())
		{
			std::cerr << "No listening port configured" << std::endl;
			return 1;
		}

		Loop loop(listeningServers, servers);
		loop.run();
		cleanupServers(listeningServers);
	}
	catch (const std::exception& e)
	{
		cleanupServers(listeningServers);
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}