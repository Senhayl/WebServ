/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 17:53:51 by aaiache           #+#    #+#             */
/*   Updated: 2026/02/04 19:32:55 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

Server::Server(int port) : _port(port)
{
    setUpSocket();
}

Server::~Server()
{
    close(_fd);
}

int Server::getFd() const
{
    return _fd;
}

const std::vector<pollfd>& Server::getPollFds() const
{
    return _fds;
}

void Server::setUpSocket()
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
    _addr.sin_port = htons(_port);
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
	std::cout << "Server listening on port " << _port << "\n";
}