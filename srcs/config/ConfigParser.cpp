#include "ConfigParser.hpp"
#include <sstream>




const std::vector<std::string>& ConfigParser::getErrors() const {
	return _errors;
}

//Readfile from filepath and copy it into a string
std::string ConfigParser::readFile(const std::string& filepath) {
	std::ifstream file(filepath.c_str());

	if (!file.is_open())
		throw std::runtime_error("Cannot open config file");

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

// Tokenize everyword and every special character
std::vector<std::string> ConfigParser::tokenize(std::string content){
	std::vector<std::string> tokens;
	std::string currentToken;
	size_t i = 0;
	while(i < content.length())
	{
		if(content[i] == '#')
		{
			while (content[i] != '\n' && i < content.length())
				i++;
		i++;
		continue;
		}
		if (content[i] == ' ' || content[i] == '\n' || content[i] == '\t')
		{
			if (!currentToken.empty())
			{
				tokens.push_back(currentToken);
				currentToken = "";
			}
			i++;
			continue;
		}
		if (content[i] == ';' || content[i] == '{' || content[i] == '}')
		{
			if (!currentToken.empty())
			{
				tokens.push_back(currentToken);
				currentToken = "";
			}
			tokens.push_back(std::string(1,content[i]));
			i++;
			continue;
		}
		currentToken += content[i];
		i++;
	}
	return tokens;
}

// Fullfill Location Class from tokens content
Location ConfigParser::parseLocationBlock(std::vector<std::string>::iterator& it){
	Location location;
	location.setPath(*it);
	while (*it != "}")
	{
		if (*it == "allowed_methods")
		{
			it++;
			while (*it != ";")
			{
				location.addAllowedMethod(*it);
				++it;
			}
		}
		else if (*it == "root")
		{
			++it;
			location.setRoot(*it);
			++it;
		}
		else if (*it == "index")
		{
			++it;
			location.setIndex(*it);
			++it;
		}
		else if (*it == "return")
		{
			++it;
			location.setReturn(*it);
			++it;
		}
		else if (*it == "upload_path")
		{
			++it;
			location.setUploadPath(*it);
			++it;
		}
		else if (*it == "cgi_extension")
		{
			++it;
			location.setCgiExtensions(*it);
			it++;
		}
		else if (*it == "cgi_path")
		{
			++it;
			location.setCgiPath(*it);
			it++;
		}
		else if (*it == "client_max_body_size")
		{
			++it;
			size_t MBS = std::atoi(it->c_str());
			location.setClientMaxBodySize(MBS);
			it++;
		}
		else if (*it == "autoindex")
        {
            ++it;
            if (*it == ";" || *it == "}")
            {
				_errors.push_back(std::string("location '") + location.getPath() + "': autoindex missing value (expected 'on' or 'off')");
                location.setAutoindex(false);
            }
            else if (*it == "on" || *it == "off")
            {
                location.setAutoindex(*it == "on");
                ++it;
            }
            else
            {
				_errors.push_back(std::string("location '") + location.getPath() + "': autoindex invalid value '" + *it + "' (expected 'on' or 'off')");
                location.setAutoindex(false);
                ++it;
            }
		}
		it++;
	}
	return location;
}

//Fullfill Serverdata from tokens content
Server ConfigParser::parseServerBlock(std::vector<std::string>::iterator& it){
	Server server;
	it++;
	it++;
	while (*it != "}")
	{
		if (*it == "listen")
		{
			++it;
			while (*it != ";")
			{
				int port = std::atoi(it->c_str());	
				server.addListen(port);
				++it;
			}
		}
		else if (*it == "server_name")
		{
			++it;
			while (*it != ";")
			{
				server.addServerName(*it);
				++it;
			}
		}
		else if (*it == "root")
		{
			++it;
			while (*it != ";")
			{
				server.setRoot(*it);
				it++;
			}
		}
		else if (*it == "index")
		{
			++it;
			while (*it != ";")
			{
				server.setIndex(*it);
				it++;
			}
		}
		else if (*it == "client_max_body_size")
		{
			it++;
			size_t MBS = std::atol(it->c_str());
			server.setMaxBodySize(MBS);
			it++;
		}
		else if (*it == "error_page")
		{
			it++;
			int error_number = std::atoi(it->c_str());
			it++;
			std::string error_html = *it;
			std::map<int, std::string> error_page;
			error_page.insert(std::make_pair(error_number, error_html));
			server.setErrorPage(error_page);
			it++;
		}
		else if (*it == "location")
		{
			it++;
			Location location = parseLocationBlock(it);
			server.addLocation(location);
		}
		it++;
	}
	return server;
}

// Main parsing functions that call all reading & fullfilling functions 
Config ConfigParser::parse(const std::string& filepath)
{
	Config config;
	std::string content = this->readFile(filepath);
	
	std::vector<std::string> tokens = tokenize(content);

	std::vector<std::string>::iterator it = tokens.begin();
	while (it != tokens.end())
	{
		if (*it == "server")
		{
			Server server = parseServerBlock(it);
			config.addServer(server);
		}
		else
			it++;
	}
	return config;
}
