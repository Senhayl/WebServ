#include "HttpRequest.hpp"
#include "RequestParser.hpp"
#include "RequestValidator.hpp"
#include "../Response/HttpResponse.hpp"
#include "../Response/MethodHandler.hpp"

int main() {
	std::string rawRequest = "POST /index.js HTTP/1.1\r\nHost: test.com\r\nAccept: text/html\r\n\r\n";
	
	RequestParser parser;
	HttpRequest request = parser.parse(rawRequest);
	
	RequestValidator validator;
	bool isValid = validator.validate(request);
	
	request.print();
	
	HttpResponse response;
	
	if (!isValid) {
		response = HttpResponse::createError(request.getStatusCode());
	} else {
		response = MethodHandler::handlerGET(request);
	}
	
	std::cout << response.toString();
	
	return 0;
}