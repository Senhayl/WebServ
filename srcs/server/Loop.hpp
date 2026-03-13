/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouron <mlouron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:07:01 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/13 14:13:37 by mlouron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOOP_HPP
#define LOOP_HPP
#pragma once

#include <vector>
#include <map>
#include <set>
#include <poll.h>
#include "Server.hpp"
#include "Client.hpp"
#include "../config/ServerConfig.hpp"

class Loop
{
	private:
		std::vector<Server*> _runtimeServers;
		std::vector<ServerConfig> _servers;
		std::vector<pollfd> _fds;
		std::map<int, Client*> _clients;
		std::set<int> _listenFds;
	
		void acceptClient(int listenFd);
		void handleClientRead(size_t index);
		void handleClientWrite(size_t index);
		void removeClient(size_t index);
		void removeTimedOutClients();

	public:
		Loop(const std::vector<Server*>& runtimeServers, const std::vector<ServerConfig>& servers);
		void run();
};

#endif