#pragma once

#include "Response/MethodHandler.hpp"
#include "Response/HttpResponse.hpp"
#include "Request/HttpRequest.hpp"
#include "Request/RequestParser.hpp"
#include "Request/RequestValidator.hpp"

std::string getAnswer(std::string rawRequest) {
	RequestParser parser;
	HttpRequest request = parser.parse(rawRequest);
	request.print();
	
	RequestValidator validator;
	bool isValid = validator.validate(request);
	
	HttpResponse response;
	
	if (!isValid) {
		response = HttpResponse::createError(request.getStatusCode());
	} else {
		if (request.getMethod() == "GET")
			response = MethodHandler::handlerGET(request);
		else if (request.getMethod() == "POST")
			response = MethodHandler::handlerPOST(request);
		else if (request.getMethod() == "DELETE")
			response = MethodHandler::handlerDELETE(request);
	}
	
	return response.toString();
}