/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:05:11 by aaiache           #+#    #+#             */
/*   Updated: 2026/02/04 19:17:00 by aaiache          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client
{
	private:
		int _fd;
		std::string _buffer;
		std::string _response;

	public:
		Client(int fd);
		~Client();

		int getFd() const;
		std::string& getBuffer();
		void setResponse(const std::string& response);
		const std::string& getResponse() const;
		void clearResponse();
};

#endif