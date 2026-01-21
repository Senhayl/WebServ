/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLoop.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:55:08 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/21 16:55:28 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerLoop.hpp"

ServerLoop::ServerLoop() {}
ServerLoop::~ServerLoop() {}

/* SERVER SETUP:
[1]Creation du socket
[2]Allocation et setup de l'adresse
[3]Bind
[4]Listen */
void ServerLoop::setup(void)
{
	//[1]AF_INET = IPV4, STOCK_STREAM = TCP, 0 =Protocol TCP par default.
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
		throw std::runtime_error("Error: could not create socket.");

	/*[2]sin_family = type d'adresse IPV4, sin_port = le port*, INADRR_ANY = localhost, toutes interfaces.
	*Le réseau utilise le big endian, alors que l’architecture de la machine peut être little endian,
		htons -> hexa to network small */
    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(8080);
    _addr.sin_addr.s_addr = INADDR_ANY;

	//[3]Bind le fd avec l'adresse: BIND(socket, adresse IPV4 castee, taille de la structure).
    if (bind(_fd, (sockaddr*)&_addr, sizeof(_addr)))
		throw std::runtime_error("Error: could not bind port.");

	//[4]Passe le socket en mode serveur, creer une file d'attente de connexions
    if (listen(_fd, 10) == -1)
		throw std::runtime_error("Error: could not listen");

	_server_poll.fd = _fd; //s-e
	_server_poll.events = POLLIN;// donnees en attente de lecture
	_fds.push_back(_server_poll);
	std::cout << "Server listening on port " << "8080\n";
while (true)
    {
        poll(&_fds[0], _fds.size(), -1);//-1 = delai, poll = attendre un evenement concernant ce fd.

        for (size_t i = 0; i < _fds.size(); i++)
        {
            if (_fds[i].revents & POLLIN)
            {
                // Nouveau client
                if (_fds[i].fd == _fd)
                {
                    int client_fd = accept(_fd, NULL, NULL);
                    pollfd client_poll;
                    client_poll.fd = client_fd;
                    client_poll.events = POLLIN;
                    _fds.push_back(client_poll);
                    std::cout << "New client connected\n";
                }
                else
                {
                    char buffer[1024];
                    int bytes = recv(_fds[i].fd, buffer, 1024, 0);
                    if (bytes <= 0)
                    {
                        close(_fds[i].fd);
                        _fds.erase(_fds.begin() + i);
                        i--;
                        std::cout << "Client disconnected\n";
                    }
                    else
                    {
                        buffer[bytes] = 0;
                        std::cout << "Received:\n" << buffer << std::endl;
                        std::string response ="HTTP/1.1 200 OK\r\nContent-Length: 12\r\nContent-Type: text/plain\r\n\r\nHello, world";
                        send(_fds[i].fd, response.c_str(), response.size(), 0);
                    }
                }
            }
        }
    }
	}