/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:28:09 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/21 15:01:47 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

class Server
{
	private:
		int _fd;
		sockaddr_in _addr; //specifique a ipv4, va servir a bind.
		std::vector<pollfd> _fds; //creer un vector de pollfd (fds, events = events attendus, revents = evenement detectes)
	    pollfd _server_poll;
	public:
		Server(/* args */);
		~Server();

		void setup();
};

#endif
