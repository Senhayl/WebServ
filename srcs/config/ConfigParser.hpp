#pragma once 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include "Config.hpp"

class ConfigParser
{
	private:
		std::string _filepath;
		std::string _content;
		std::vector<std::string> _errors;
	public:
		const std::vector<std::string>& getErrors() const;
		std::string readFile(const std::string& filepath);
		std::vector<std::string> tokenize(std::string content);
		Config parse(const std::string& filepath);
		Server parseServerBlock(std::vector<std::string>::iterator& it);
		Location parseLocationBlock(std::vector<std::string>::iterator& it);
};