#include "HttpRequest.hpp"
#include "RequestParser.hpp"
#include "RequestValidator.hpp"
#include "../Response/HttpResponse.hpp"

int main() {
	std::string rawRequest = "Youpi /index.html HTTP/1.1\r\nHost: test.com\r\nAccept: text/html\r\n\r\n";
	
	RequestParser parser;
	HttpRequest request = parser.parse(rawRequest);
	
	RequestValidator validator;
	bool isValid = validator.validate(request);
	
	request.print();
	
	HttpResponse response;
	
	if (!isValid) {
		// Réponse d'erreur
		response.setStatusCode(request.getStatusCode());
		response.setStatusMessage("Error");
		response.setBody("<html><body><h1>Error</h1></body></html>");
		response.addHeader("Content-Type", "text/html");
		response.addHeader("Content-Length", "42");
	} else {
		// Réponse OK
		response.setStatusCode(200);
		response.setStatusMessage("OK");
		response.setBody("<html><body><h1>Hello World</h1></body></html>");
		response.addHeader("Content-Type", "text/html");
		response.addHeader("Content-Length", "47");
		response.addHeader("Connection", "keep-alive");
	}
	
	response.print();
	std::cout << "\n=== Complete HTTP Response ===\n";
	std::cout << response.toString();
	std::cout << "==============================\n";
	
	return 0;
}