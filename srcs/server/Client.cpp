/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:05:09 by aaiache           #+#    #+#             */
/*   Updated: 2026/02/04 19:16:22 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd) : _fd(fd) {}

Client::~Client()
{
    close(_fd);
}

int Client::getFd() const
{
    return _fd;
}

void Client::setResponse(const std::string& response)
{
    _response = response;
}

const std::string& Client::getResponse() const
{
    return _response;
}

void Client::clearResponse()
{
    _response.clear();
}

std::string& Client::getBuffer()
{
    return _buffer;
}
