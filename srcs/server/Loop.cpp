/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:06:58 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/17 14:15:36 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <unistd.h>
#include "../http/httpHeader.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <csignal>

static volatile sig_atomic_t g_stop = 0;

static void signalHandler(int) {
	g_stop = 1;
}

Loop::Loop(const std::vector<Server*>& listeningServers, const std::vector<ServerConfig>& servers)
	: _listeningServers(listeningServers), _servers(servers)
{
	for (size_t i = 0; i < _listeningServers.size(); ++i)
	{
		const std::vector<pollfd>& serverPolls = _listeningServers[i]->getPollFds();
		for (size_t j = 0; j < serverPolls.size(); ++j)
		{
			_fds.push_back(serverPolls[j]);
			_serverFds.insert(serverPolls[j].fd);
		}
	}
}

Loop::~Loop()
{
	cleanupClients();
}

bool Loop::isServerFd(int fd) const
{
	return _serverFds.find(fd) != _serverFds.end();
}

void Loop::acceptClient(int serverFd)
{
	int client_fd = accept(serverFd, NULL, NULL);
	if (client_fd < 0)
		return;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	pollfd client_poll;
	client_poll.fd = client_fd;
	client_poll.events = POLLIN | POLLOUT;
	client_poll.revents = 0;

	_fds.push_back(client_poll);
	_clients[client_fd] = new Client(client_fd);
}

void Loop::handleClientRead(size_t index)
{
	char buffer[4096];
	int fd = _fds[index].fd;

	ssize_t bytes = read(fd, buffer, sizeof(buffer));
	if (bytes <= 0)
	{
		removeClient(index);
		return;
	}

	_clients[fd]->getBuffer().append(buffer, bytes);

	std::string& buff = _clients[fd]->getBuffer();
	size_t headerEnd = buff.find("\r\n\r\n");
	
	if (headerEnd != std::string::npos)
	{
		// Headers are complete, check if we have the full body
		size_t contentLength = 0;
		size_t clPos = buff.find("Content-Length: ");
		if (clPos != std::string::npos && clPos < headerEnd)
		{
			size_t clStart = clPos + 16; // Length of "Content-Length: "
			size_t clEnd = buff.find("\r\n", clStart);
			std::string clValue = buff.substr(clStart, clEnd - clStart);
			contentLength = atoi(clValue.c_str());
		}
		
		size_t bodyStart = headerEnd + 4; // After "\r\n\r\n"
		size_t currentBodySize = buff.size() - bodyStart;
		
		// Only process if we have the complete body
		if (currentBodySize >= contentLength)
		{
			std::string response = getAnswer(buff, this->_servers);
			_clients[fd]->setResponse(response);
			_clients[fd]->getBuffer().clear();
		}
	}
}

void Loop::removeClient(size_t index)
{
	int fd = _fds[index].fd;

	delete _clients[fd];
	_clients.erase(fd);
	_fds.erase(_fds.begin() + index);
}

void Loop::cleanupClients()
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
	_clients.clear();

	for (std::vector<pollfd>::iterator it = _fds.begin(); it != _fds.end();)
	{
		if (!isServerFd(it->fd))
			it = _fds.erase(it);
		else
			++it;
	}
}

void Loop::handleClientWrite(size_t index)
{
	int fd = _fds[index].fd;
	const std::string& response = _clients[fd]->getResponse();
	if (response.empty())
		return;
	
	ssize_t sent = send(fd, response.c_str(), response.length(), 0);
	if (sent <= 0)
	{
		removeClient(index);
		return;
	}
	if (response.find("Connection: close") != std::string::npos)
	{
		removeClient(index);
	}
	else
	{
		_clients[fd]->clearResponse();
	}
}

void Loop::run()
{
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	while (!g_stop)
	{
		if (poll(&_fds[0], _fds.size(), -1) < 0)
			continue;
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				if (!isServerFd(_fds[i].fd))
					removeClient(i--);
				continue;
			}
			if (_fds[i].revents & POLLIN)
			{
				if (isServerFd(_fds[i].fd))
					acceptClient(_fds[i].fd);
				else
					handleClientRead(i);
			}
			if (i < _fds.size() && (_fds[i].revents & POLLOUT) && !isServerFd(_fds[i].fd))
				handleClientWrite(i);
		}
	}
}
