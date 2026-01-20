#include "request.hpp"

std::vector<Token> tokenize(std::string request) {
	std::string buffer;
	std::vector<Token> tokens;
	Token token;
	int count = 0;
	for (size_t i = 0; i < request.length(); i++) {
		if (request[i] == ' ' || request[i] == '\n') {
			if (count == 0) {
				if (buffer == "GET") {
					token._type = "METHOD";
					token._value = buffer;
				} else if (buffer == "POST") {
					token._type = "METHOD";
					token._value = buffer;
				} else if (buffer == "DELETE"){
					token._type = "METHOD";
					token._value = buffer;
				} else {
					std::cerr << "Invalid method\n";
					exit(1);
				}
			} else if (count == 1) {
				token._type = "PATH";
				token._value = buffer;
			} else if (count == 2) {
				token._type = "VERSION";
				token._value = buffer;
			}
			tokens.push_back(token);
			buffer.clear();
			count++;
		} else
			buffer.push_back(request[i]);
	}
	return tokens;
}

int main() {
	std::vector<Token> tokens = tokenize("okdsokfodkfsodkf /index.html HTTP1.1\n");
	for (size_t i = 0; i < tokens.size(); i++) {
		std::cout << tokens[i]._type << " " << tokens[i]._value << std::endl;
	}
}