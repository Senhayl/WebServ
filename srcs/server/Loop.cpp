/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:06:58 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/22 18:10:29 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <unistd.h>
#include <fcntl.h>

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

    //HTTP parsing
}

void Loop::removeClient(size_t index)
{
    int fd = _fds[index].fd;

    delete _clients[fd];
    _clients.erase(fd);

    close(fd);
    _fds.erase(_fds.begin() + index);
}



void Loop::run()
{
    while (true)
    {
        if (poll(&_fds[0], _fds.size(), -1) < 0)
            continue;

        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLIN)
            {
                if (_fds[i].fd == _server.getFd())
                    acceptClient();
                else
                    handleClientRead(i);
            }
        }
    }
}
