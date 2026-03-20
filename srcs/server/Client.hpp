/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:05:11 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/20 12:51:48 by aaiache          ###   ########.fr       */
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
		time_t _lastActivity;
		time_t _requestStart;
		bool _timedOut;

	public:
		Client(int fd);
		~Client();

		int getFd() const;
		std::string& getBuffer();
		void setResponse(const std::string& response);
		const std::string& getResponse() const;
		void clearResponse();
		void touchActivity();
		void resetRequestTimer();
		time_t getLastActivity() const;
		time_t getRequestStart() const;
		bool isTimedOut() const;
		void setTimedOut(bool timedOut);
};

#endif