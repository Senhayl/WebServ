/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 17:53:48 by aaiache           #+#    #+#             */
/*   Updated: 2026/01/22 18:04:33 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>
#include <vector>
#include <poll.h>

class Server
{
	private:
		int _fd;
		int _port;
	
		sockaddr_in _addr;
		pollfd _server_poll;
		std::vector<pollfd> _fds;
	
		void setUpSocket();

	public:
		Server(int port);
		~Server();

		int getFd() const;
		const std::vector<pollfd>& getPollFds() const;
};


#endif