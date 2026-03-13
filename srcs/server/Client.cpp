/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouron <mlouron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:05:09 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/13 14:04:28 by mlouron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd) : _fd(fd), _sentBytes(0), _lastActivity(std::time(NULL)) {}

Client::~Client()
{
}

int Client::getFd() const
{
    return _fd;
}

void Client::setResponse(const std::string& response)
{
    _response = response;
    _sentBytes = 0;
}

const std::string& Client::getResponse() const
{
    return _response;
}

void Client::clearResponse()
{
    _response.clear();
    _sentBytes = 0;
}

size_t Client::getSentBytes() const
{
    return _sentBytes;
}

void Client::addSentBytes(size_t sent)
{
    _sentBytes += sent;
}

void Client::touchActivity()
{
	_lastActivity = std::time(NULL);
}

time_t Client::getLastActivity() const
{
	return _lastActivity;
}

std::string& Client::getBuffer()
{
    return _buffer;
}
