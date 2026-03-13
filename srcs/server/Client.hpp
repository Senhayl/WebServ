/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouron <mlouron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:05:11 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/13 14:04:28 by mlouron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <ctime>

class Client
{
	private:
		int _fd;
		std::string _buffer;
		std::string _response;
		size_t _sentBytes;
		time_t _lastActivity;

	public:
		Client(int fd);
		~Client();

		int getFd() const;
		std::string& getBuffer();
		void setResponse(const std::string& response);
		const std::string& getResponse() const;
		void clearResponse();
		size_t getSentBytes() const;
		void addSentBytes(size_t sent);
		void touchActivity();
		time_t getLastActivity() const;
};

#endif