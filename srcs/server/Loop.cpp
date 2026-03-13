/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouron <mlouron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 18:06:58 by aaiache           #+#    #+#             */
/*   Updated: 2026/03/13 14:13:37 by mlouron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Loop.hpp"
#include <unistd.h>
#include "../http/httpHeader.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <ctime>

static const int kPollTimeoutMs = 1000;
static const int kClientIdleTimeoutSec = 30;

static std::string toLowerStr(const std::string& s)
{
	std::string out = s;
	for (size_t i = 0; i < out.size(); ++i)
		out[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(out[i])));
	return out;
}

static bool isChunkedRequest(const std::string& headerBlock)
{
	std::string lower = toLowerStr(headerBlock);
	return lower.find("transfer-encoding: chunked") != std::string::npos;
}

static size_t parseContentLength(const std::string& headerBlock)
{
	std::string lower = toLowerStr(headerBlock);
	const std::string key = "content-length:";
	size_t pos = lower.find(key);
	if (pos == std::string::npos)
		return 0;

	pos += key.size();
	while (pos < lower.size() && (lower[pos] == ' ' || lower[pos] == '\t'))
		++pos;

	size_t end = lower.find("\r\n", pos);
	if (end == std::string::npos)
		end = lower.size();

	std::string value = lower.substr(pos, end - pos);
	if (value.empty())
		return 0;

	for (size_t i = 0; i < value.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(value[i])))
			return 0;
	}

	return static_cast<size_t>(std::strtoul(value.c_str(), NULL, 10));
}

enum ChunkDecodeStatus
{
	CHUNK_INCOMPLETE,
	CHUNK_INVALID,
	CHUNK_OK
};

static ChunkDecodeStatus decodeChunkedBody(const std::string& body, std::string& decoded)
{
	decoded.clear();
	size_t pos = 0;

	while (true)
	{
		size_t lineEnd = body.find("\r\n", pos);
		if (lineEnd == std::string::npos)
			return CHUNK_INCOMPLETE;

		std::string sizeLine = body.substr(pos, lineEnd - pos);
		size_t semiPos = sizeLine.find(';');
		if (semiPos != std::string::npos)
			sizeLine = sizeLine.substr(0, semiPos);
		if (sizeLine.empty())
			return CHUNK_INVALID;

		char* endPtr = NULL;
		unsigned long chunkSize = std::strtoul(sizeLine.c_str(), &endPtr, 16);
		if (endPtr == sizeLine.c_str() || *endPtr != '\0')
			return CHUNK_INVALID;

		pos = lineEnd + 2;
		if (body.size() < pos + chunkSize + 2)
			return CHUNK_INCOMPLETE;

		if (chunkSize > 0)
			decoded.append(body, pos, chunkSize);

		pos += chunkSize;
		if (body.compare(pos, 2, "\r\n") != 0)
			return CHUNK_INVALID;
		pos += 2;

		if (chunkSize == 0)
			return (pos == body.size()) ? CHUNK_OK : CHUNK_INVALID;
	}
}

static std::string normalizeChunkedRequest(const std::string& rawRequest, size_t headerEnd, const std::string& decodedBody)
{
	std::string headerBlock = rawRequest.substr(0, headerEnd);
	std::ostringstream out;

	size_t lineEnd = headerBlock.find("\r\n");
	if (lineEnd == std::string::npos)
		return rawRequest;

	out << headerBlock.substr(0, lineEnd) << "\r\n";

	size_t pos = lineEnd + 2;
	while (pos < headerBlock.size())
	{
		size_t nextEnd = headerBlock.find("\r\n", pos);
		if (nextEnd == std::string::npos)
			nextEnd = headerBlock.size();

		std::string line = headerBlock.substr(pos, nextEnd - pos);
		size_t colon = line.find(':');
		if (colon != std::string::npos)
		{
			std::string key = toLowerStr(line.substr(0, colon));
			if (key != "transfer-encoding" && key != "content-length")
				out << line << "\r\n";
		}

		pos = nextEnd + 2;
	}

	out << "Content-Length: " << decodedBody.size() << "\r\n\r\n";
	out << decodedBody;
	return out.str();
}

Loop::Loop(const std::vector<Server*>& runtimeServers, const std::vector<ServerConfig>& servers)
	: _runtimeServers(runtimeServers), _servers(servers)
{
	for (size_t i = 0; i < _runtimeServers.size(); ++i)
	{
		const std::vector<pollfd>& sFds = _runtimeServers[i]->getPollFds();
		for (size_t j = 0; j < sFds.size(); ++j)
		{
			_fds.push_back(sFds[j]);
			_listenFds.insert(sFds[j].fd);
		}
	}
}

void Loop::acceptClient(int listenFd)
{
	int client_fd = accept(listenFd, NULL, NULL);
	if (client_fd < 0)
		return;

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	pollfd client_poll;
	client_poll.fd = client_fd;
	client_poll.events = POLLIN | POLLOUT;
	client_poll.revents = 0;

	_fds.push_back(client_poll);
	_clients[client_fd] = new Client(client_fd);
}

void Loop::handleClientRead(size_t index)
{
	char buffer[4096];
	int fd = _fds[index].fd;

	ssize_t bytes = read(fd, buffer, sizeof(buffer));
	if (bytes <= 0)
	{
		removeClient(index);
		return;
	}

	_clients[fd]->touchActivity();
	_clients[fd]->getBuffer().append(buffer, bytes);

	std::string& buff = _clients[fd]->getBuffer();
	size_t headerEnd = buff.find("\r\n\r\n");
	if (headerEnd == std::string::npos)
		return;

	std::string headerBlock = buff.substr(0, headerEnd);
	if (isChunkedRequest(headerBlock))
	{
		std::string body = buff.substr(headerEnd + 4);
		std::string decoded;
		ChunkDecodeStatus decodeStatus = decodeChunkedBody(body, decoded);
		if (decodeStatus == CHUNK_INCOMPLETE)
			return;
		if (decodeStatus == CHUNK_INVALID)
		{
			_clients[fd]->setResponse(HttpResponse::createError(400).toString());
			_clients[fd]->getBuffer().clear();
			return;
		}

		std::string normalized = normalizeChunkedRequest(buff, headerEnd, decoded);
		_clients[fd]->setResponse(getAnswer(normalized, _servers));
		_clients[fd]->getBuffer().clear();
		return;
	}

	size_t contentLength = parseContentLength(headerBlock);
	size_t bodyStart = headerEnd + 4;
	size_t currentBodySize = buff.size() - bodyStart;
	if (currentBodySize >= contentLength)
	{
		_clients[fd]->setResponse(getAnswer(buff, _servers));
		_clients[fd]->getBuffer().clear();
	}
}

void Loop::removeClient(size_t index)
{
	int fd = _fds[index].fd;

	delete _clients[fd];
	_clients.erase(fd);

	close(fd);
	_fds.erase(_fds.begin() + index);
}

void Loop::handleClientWrite(size_t index)
{
	int fd = _fds[index].fd;
	const std::string& response = _clients[fd]->getResponse();
	if (response.empty())
		return;

	_clients[fd]->touchActivity();

	size_t sentBytes = _clients[fd]->getSentBytes();
	if (sentBytes >= response.size())
	{
		if (response.find("Connection: close") != std::string::npos)
			removeClient(index);
		else
			_clients[fd]->clearResponse();
		return;
	}

	ssize_t sent = send(fd, response.c_str() + sentBytes, response.length() - sentBytes, 0);
	if (sent <= 0)
	{
		removeClient(index);
		return;
	}
	_clients[fd]->addSentBytes(static_cast<size_t>(sent));

	if (_clients[fd]->getSentBytes() < response.size())
		return;

	if (response.find("Connection: close") != std::string::npos)
		removeClient(index);
	else
		_clients[fd]->clearResponse();
}

void Loop::removeTimedOutClients()
{
	time_t now = std::time(NULL);
	for (size_t i = 0; i < _fds.size(); ++i)
	{
		int fd = _fds[i].fd;
		if (_listenFds.find(fd) != _listenFds.end())
			continue;

		std::map<int, Client*>::iterator it = _clients.find(fd);
		if (it == _clients.end())
			continue;

		if (now - it->second->getLastActivity() > kClientIdleTimeoutSec)
			removeClient(i--);
	}
}

void Loop::run()
{
	while (true)
	{
		removeTimedOutClients();
		if (_fds.empty())
			continue;
		if (poll(&_fds[0], _fds.size(), kPollTimeoutMs) < 0)
			continue;
		for (size_t i = 0; i < _fds.size(); i++)
		{
			if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL))
			{
				if (_listenFds.find(_fds[i].fd) == _listenFds.end())
					removeClient(i--);
				continue;
			}
			if (_fds[i].revents & POLLIN)
			{
				if (_listenFds.find(_fds[i].fd) != _listenFds.end())
					acceptClient(_fds[i].fd);
				else
					handleClientRead(i);
			}
			if (_fds[i].revents & POLLOUT)
			{
				if (_listenFds.find(_fds[i].fd) == _listenFds.end())
					handleClientWrite(i);
			}
		}
	}
}
