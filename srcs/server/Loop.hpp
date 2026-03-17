/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:07:01 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/17 14:15:18 by aaiache          ###   ########.fr       */
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
		std::vector<Server*> _listeningServers;
		std::set<int> _serverFds;
		std::vector<ServerConfig> _servers;
		std::vector<pollfd> _fds;
		std::map<int, Client*> _clients;
	
		void acceptClient(int serverFd);
		void handleClientRead(size_t index);
		void handleClientWrite(size_t index);
		void removeClient(size_t index);
		void cleanupClients();
		bool isServerFd(int fd) const;

	public:
		Loop(const std::vector<Server*>& listeningServers, const std::vector<ServerConfig>& servers);
		~Loop();
		void run();
};

#endif