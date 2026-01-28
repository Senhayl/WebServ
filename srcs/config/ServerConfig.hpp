#pragma once 
#include <iostream>
#include <map>
#include <vector>
#include "Location.hpp"


class ServerConfig {

	private:
		std::vector<int> _listen;
		std::vector<std::string> _serverName;
		std::string _root;
		std::string _index;
		size_t _clientMaxBodySize;
		std::map<int, std::string> _errorPage;
		std::vector<Location> _locations;

	public:
		void setListen(const std::vector<int>& listen);
		void addListen(const int port);
		void setServerName(const std::vector<std::string>& serverName);
		void addServerName(const std::string& name);
		void setRoot(const std::string& root);
		void setIndex(const std::string& index);
		void setMaxBodySize(const size_t& MBS);
		void setErrorPage(const std::map<int, std::string>& errorPage);
		void setLocation(const std::vector<Location>& location);
		void addLocation(const Location& location);

		const std::vector<int>& getListen() const ;
		const std::vector<std::string>& getServerName() const ;
		const std::string& getRoot() const ;
		const std::string& getIndex() const ;
		const size_t& getMBS() const ;
		const std::map<int, std::string>& getErrorPage() const ;
		const std::vector<Location>& getLocation() const;
};