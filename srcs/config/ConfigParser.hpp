#pragma once 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Config.hpp"

class ConfigParser
{
	private:
		std::string _filepath;
		std::string _content;
	public:
		std::string readFile(const std::string& filepath);
		std::vector<std::string> tokenize(std::string content);
		Config parse(const std::string& filepath);
		ServerConfig parseServerBlock(std::vector<std::string>::iterator& it);
};