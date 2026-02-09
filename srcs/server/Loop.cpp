/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shessoun <shessoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:06:58 by aaiache           #+#    #+#             */
/*   Updated: 2026/02/09 16:32:03 by shessoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <unistd.h>
#include "../http/httpHeader.hpp"
#include <fcntl.h>
#include <sys/socket.h>

Loop::Loop(Server& server) : _server(server)
{
    _fds = server.getPollFds();
}
void Loop::acceptClient()
{
    int client_fd = accept(_server.getFd(), NULL, NULL);
    if (client_fd < 0)
        return;

    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    pollfd client_poll;
    client_poll.fd = client_fd;
    client_poll.events = POLLIN;
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
            std::string response = getAnswer(buff);
            _clients[fd]->setResponse(response);
            _clients[fd]->getBuffer().clear();
            _fds[index].events = POLLOUT;
        }
    }
}

void Loop::removeClient(size_t index)
{
    int fd = _fds[index].fd;

    delete _clients[fd];
    _clients.erase(fd);

    close(fd);
    _fds.erase(_fds.begin() + index);
}

void Loop::handleClientWrite(size_t index)
{
    int fd = _fds[index].fd;
    const std::string& response = _clients[fd]->getResponse();
    
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
        _fds[index].events = POLLIN;
    }
}

void Loop::run()
{
    while (true)
    {
        if (poll(&_fds[0], _fds.size(), -1) < 0)
            continue;
        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                if (_fds[i].fd != _server.getFd())
                    removeClient(i--);
                continue;
            }
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _server.getFd())
                    acceptClient();
                else
                    handleClientRead(i);
            }
            else if (_fds[i].revents & POLLOUT)
                handleClientWrite(i);
        }
    }
}
