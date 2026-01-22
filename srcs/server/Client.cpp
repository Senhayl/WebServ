/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:05:09 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/22 18:06:11 by aaiache          ###   ########.fr       */
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

std::string& Client::getBuffer()
{
    return _buffer;
}
