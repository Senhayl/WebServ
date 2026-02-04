/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c++                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:37:46 by aaiache           #+#    #+#             */
/*   Updated: 2026/02/04 19:37:47 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/ConfigParser.hpp"
#include "server/Server.hpp"
#include "server/Loop.hpp"
#include <iostream>
#include <vector>
#include <string>

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
        std::cout << "Starting " << servers.size() << " server(s)..." << std::endl;
        
        //1 serveur et 1 port pour linstant. need plusieurs servers sur plusieurs ports?
        if (servers.empty() || servers[0].getListen().empty())
        {
            std::cerr << "No server configured" << std::endl;
            return 1;
        }
        
        int port = servers[0].getListen()[0];
        std::cout << "Server listening on port " << port << std::endl;
        
        Server server(port);
        Loop loop(server);
        loop.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}