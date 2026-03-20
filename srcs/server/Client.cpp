/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:05:09 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/20 12:51:48 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>

Client::Client(int fd)
    : _fd(fd), _lastActivity(time(NULL)), _requestStart(time(NULL)), _timedOut(false) {}

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
    _timedOut = false;
    _requestStart = time(NULL);
}

std::string& Client::getBuffer()
{
    return _buffer;
}

void Client::touchActivity()
{
    _lastActivity = time(NULL);
}

void Client::resetRequestTimer()
{
    _requestStart = time(NULL);
}

time_t Client::getLastActivity() const
{
    return _lastActivity;
}

time_t Client::getRequestStart() const
{
    return _requestStart;
}

bool Client::isTimedOut() const
{
    return _timedOut;
}

void Client::setTimedOut(bool timedOut)
{
    _timedOut = timedOut;
}
