#include "ServerConfig.hpp"
#include <set>
#include <sstream>

void ServerConfig::setListen(const std::vector<int>& listen){
    _listen = listen;
}

void ServerConfig::addListen(int port){
	_listen.push_back(port);
}

void ServerConfig::setServerName(const std::vector<std::string>& serverName){
	_serverName = serverName;
}

void ServerConfig::addServerName(const std::string& name){
	_serverName.push_back(name);
}

void ServerConfig::setRoot(const std::string& root){
	_root = root;
}

void ServerConfig::setIndex(const std::string& index){
	_index = index;
}

void ServerConfig::setMaxBodySize(const size_t& MBS){
	_clientMaxBodySize = MBS;
}

void ServerConfig::setErrorPage(const std::map<int, std::string>& errorPage){
	_errorPage = errorPage;
}

void ServerConfig::setLocation(const std::vector<Location>& location){
	_locations = location;
}

void ServerConfig::addLocation(const Location& location){
	_locations.push_back(location);
}

const std::vector<int>& ServerConfig::getListen() const {
	return _listen;
}

const std::vector<std::string>& ServerConfig::getServerName() const {
	return _serverName;
}
const std::string& ServerConfig::getRoot() const {
	return _root;
}
const std::string& ServerConfig::getIndex() const {
	return _index;
}
const size_t& ServerConfig::getMBS() const {
	return _clientMaxBodySize;
}
const std::map<int, std::string>& ServerConfig::getErrorPage() const {
	return _errorPage;
}
const std::vector<Location>& ServerConfig::getLocation() const {
	return _locations;
}

bool ServerConfig::isDataValid() const{
	std::vector<std::string> errs;
	return isDataValid(errs);
}

bool ServerConfig::isDataValid(std::vector<std::string>& errs) const
{
    if (_listen.empty())
        errs.push_back("server: listen is empty");
    else
    {
        std::set<int> seenPorts;
        for (size_t i = 0; i < _listen.size(); ++i)
        {
            const int p = _listen[i];

            if (p < 1 || p > 65535)
            {
                std::ostringstream oss;
                oss << "server: listen port out of range (1..65535): " << p;
                errs.push_back(oss.str());
            }

            if (!seenPorts.insert(p).second)
            {
                std::ostringstream oss;
                oss << "server: duplicate listen port: " << p;
                errs.push_back(oss.str());
            }
        }
    }

	{
        std::set<std::string> seenNames;
        for (size_t i = 0; i < _serverName.size(); ++i)
        {
            const std::string& name = _serverName[i];

            if (name.empty())
                errs.push_back("server: server_name contains an empty value");

            if (!name.empty() && !seenNames.insert(name).second)
            {
                std::ostringstream oss;
                oss << "server: duplicate server_name: " << name;
                errs.push_back(oss.str());
            }
        }
    }

    if (_root.empty())
        errs.push_back("server: root is empty");
    else if (_root[0] != '/')
        errs.push_back("server: root must start with '/'");
    if (!_index.empty() && _index.find('/') != std::string::npos)
        errs.push_back("server: index should be a filename (no '/')");
    if (_clientMaxBodySize == 0)
    	errs.push_back("server: client_max_body_size is missing/0");

    for (std::map<int, std::string>::const_iterator it = _errorPage.begin(); it != _errorPage.end(); ++it)
    {
        const int code = it->first;
        const std::string& path = it->second;
        if (code < 400 || code > 599)
        {
            std::ostringstream oss;
            oss << "server: error_page invalid status code (expected 400..599): " << code;
            errs.push_back(oss.str());
        }
        if (path.empty())
        {
            std::ostringstream oss;
            oss << "server: error_page path is empty for code " << code;
            errs.push_back(oss.str());
        }
        else if (path[0] != '/')
        {
            std::ostringstream oss;
            oss << "server: error_page path must start with '/' for code " << code;
            errs.push_back(oss.str());
        }
    }
    {
        std::set<std::string> seenPaths;

        for (size_t i = 0; i < _locations.size(); ++i)
        {
            const Location& loc = _locations[i];

            const std::string& p = loc.getPath();
            if (!p.empty() && !seenPaths.insert(p).second)
            {
                std::ostringstream oss;
                oss << "server: duplicate location path: " << p;
                errs.push_back(oss.str());
            }

            std::vector<std::string> locErrs;
            loc.isDataValid(locErrs);
            for (size_t k = 0; k < locErrs.size(); ++k)
            {
                std::ostringstream oss;
                oss << "server: location '" << loc.getPath() << "': " << locErrs[k];
                errs.push_back(oss.str());
            }
        }
    }

    return errs.empty();
}

// 400
// 404 
// 405
// 505
