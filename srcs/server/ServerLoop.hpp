/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLoop.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:54:35 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/21 16:54:53 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERLOOP_HPP
#define SERVERLOOP_HPP

#include <iostream>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

class ServerLoop
{
	private:
		int _fd;
		sockaddr_in _addr; //specifique a ipv4, va servir a bind.
		std::vector<pollfd> _fds; //creer un vector de pollfd (fds, events = events attendus, revents = evenement detectes)
	    pollfd _server_poll;
	public:
		ServerLoop(/* args */);
		~ServerLoop();

		void setup();
};

#endif