/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shessoun <shessoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:07:01 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/03 13:41:36 by shessoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOOP_HPP
#define LOOP_HPP
#pragma once

#include <vector>
#include <map>
#include <poll.h>
#include "Server.hpp"
#include "Client.hpp"
#include "../config/ServerConfig.hpp"

class Loop
{
	private:
		Server& _server;
		std::vector<ServerConfig> _servers;
		std::vector<pollfd> _fds;
		std::map<int, Client*> _clients;
	
		void acceptClient();
		void handleClientRead(size_t index);
		void handleClientWrite(size_t index);
		void removeClient(size_t index);

	public:
		Loop(Server& server, const std::vector<ServerConfig>& servers);
		void run();
};

#endif