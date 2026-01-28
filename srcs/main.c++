#include "config/ConfigParser.hpp"
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
		
        const std::vector<Server>& servers = config.getServers();
        for (size_t i = 0; i < servers.size(); ++i)
            servers[i].isDataValid(errs);

        if (!errs.empty())
        {
            std::cerr << "Config validation errors (" << errs.size() << "):" << std::endl;
            for (size_t i = 0; i < errs.size(); ++i)
                std::cerr << "- " << errs[i] << std::endl;
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}