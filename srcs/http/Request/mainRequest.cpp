#include "../httpHeader.hpp"

int main() {
	std::vector<Token> tokens = tokenize("GET /index.html HTTP/1.1\r\nHost: test.com\ntest: ok\ntest2: ok2\n\r\nbody slkdjfsldkjf");
	std::map<std::string, std::string> headers = mapHeaders(tokens);
	int status_code = checkRequest(tokens, headers);
	if (status_code != OK)
		std::cout << "oups\n";
	return 0;
}