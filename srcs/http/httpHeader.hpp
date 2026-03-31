#pragma once

#include "Response/MethodHandler.hpp"
#include "Response/HttpResponse.hpp"
#include "Response/CgiHandler.hpp"
#include "Request/HttpRequest.hpp"
#include "Request/RequestParser.hpp"
#include "Request/RequestValidator.hpp"

static std::string getPrimaryServerName(const ServerConfig& server) {
	const std::vector<std::string>& names = server.getServerName();
	if (!names.empty() && !names[0].empty())
		return names[0];
	return "Webserv";
}

ServerConfig findServerConfig(const std::vector<ServerConfig>& servers, std::string host) {
	std::string hostOnly = host;
	int hostPort = -1;

	size_t colonPos = host.rfind(':');
	if (colonPos != std::string::npos && host.find(']') == std::string::npos) {
		hostOnly = host.substr(0, colonPos);
		std::string portPart = host.substr(colonPos + 1);
		if (!portPart.empty()) {
			hostPort = std::atoi(portPart.c_str());
		}
	}

	for (size_t i = 0; i < servers.size(); i++) {
		if (hostPort != -1) {
			const std::vector<int>& listens = servers[i].getListen();
			bool listensOnHostPort = false;
			for (size_t p = 0; p < listens.size(); ++p) {
				if (listens[p] == hostPort) {
					listensOnHostPort = true;
					break;
				}
			}
			if (!listensOnHostPort)
				continue;
		}

		for (size_t j = 0; j < servers[i].getServerName().size(); j++) {
			if (servers[i].getServerName().at(j) == hostOnly)
				return servers[i];
		}
	}

	if (hostPort != -1) {
		for (size_t i = 0; i < servers.size(); i++) {
			const std::vector<int>& listens = servers[i].getListen();
			for (size_t p = 0; p < listens.size(); ++p) {
				if (listens[p] == hostPort)
					return servers[i];
			}
		}
	}

	for (size_t i = 0; i < servers.size(); i++) {
		for (size_t j = 0; j < servers[i].getServerName().size(); j++) {
			if (servers[i].getServerName().at(j) == hostOnly)
				return servers[i];
		}
	}

	return servers[0];
}

Location findLocation(ServerConfig server, std::string uri) {
	size_t bestLen = 0;
	Location loc;
	for (size_t i = 0; i < server.getLocation().size(); i++) {
		const std::string& locPath = server.getLocation().at(i).getPath();
		if (uri.compare(0, locPath.size(), locPath) == 0) {
			if (uri.size() == locPath.size() || uri[locPath.size()] == '/' || locPath[locPath.size() - 1] == '/') {
				if (bestLen < locPath.size()) {
					bestLen = locPath.size();
					loc = server.getLocation().at(i);
				}
			}
		}
	}
	return loc;
}

std::string getAnswer(std::string rawRequest, const std::vector<ServerConfig> servers) {
	RequestParser parser;
	HttpRequest request = parser.parse(rawRequest);
	
	ServerConfig srvConf = findServerConfig(servers, request.getHeader("Host"));
	Location servLoc = findLocation(srvConf, request.getPath());
	//au cas ou il faut rediriger avec le fichier de config
	if (!servLoc.getReturn().empty()) {
		HttpResponse resp(301);
		resp.setStatusMessage(301);
		resp.addHeader("Location", servLoc.getReturn());
		resp.addHeader("Server", getPrimaryServerName(srvConf));
		resp.addHeader("Content-Length", "0");
		resp.addHeader("Connection", "close");
		return resp.toString();
	}
	HttpResponse response;
	if (!servLoc.getAllowedMethods().empty()) {
		const std::vector<std::string>& methods = servLoc.getAllowedMethods();
		bool allowed = false;
		for (size_t i = 0; i < methods.size(); i++) {
			if (methods[i] == request.getMethod()) {
				allowed = true;
				break;
			}
		}
		if (!allowed) {
			response = HttpResponse::createError(405, srvConf);
			response.addHeader("Server", getPrimaryServerName(srvConf));
			response.print();
			return response.toString();
		}
	}
	RequestValidator validator;
	bool isValid = validator.validate(request);

	if (servLoc.getClientMaxBodySize() > 0 && request.getBody().size() > servLoc.getClientMaxBodySize()) {
		HttpResponse oversized = HttpResponse::createError(413, srvConf);
		oversized.addHeader("Server", getPrimaryServerName(srvConf));
		return oversized.toString();
	}
	
	// Détection CGI
	bool isCgi = false;
	if (!servLoc.getCgiExtensions().empty() && !servLoc.getCgiPath().empty()) {
		const std::string& ext = servLoc.getCgiExtensions();
		std::string cleanPath = request.getPath();
		size_t qpos = cleanPath.find('?');
		if (qpos != std::string::npos)
			cleanPath = cleanPath.substr(0, qpos);
		if (cleanPath.size() >= ext.size() &&
			cleanPath.substr(cleanPath.size() - ext.size()) == ext)
			isCgi = true;
	}

	if (!isValid) {
		response = HttpResponse::createError(request.getStatusCode(), srvConf);
	} else if (isCgi) {
		response = CgiHandler::execute(request, srvConf, servLoc);
	} else {
		if (request.getMethod() == "GET")
			response = MethodHandler::handlerGET(request, srvConf, servLoc);
		else if (request.getMethod() == "POST")
			response = MethodHandler::handlerPOST(request, srvConf, servLoc);
		else if (request.getMethod() == "DELETE")
			response = MethodHandler::handlerDELETE(request, srvConf, servLoc);
	}

	response.addHeader("Server", getPrimaryServerName(srvConf));
	
	response.print();
	return response.toString();
}