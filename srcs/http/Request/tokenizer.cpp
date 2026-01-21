#include "tokenizer.hpp"

static std::vector<std::string> splitRequest(std::string request) {
	std::string buffer;
	std::vector<std::string> arr;
	for (size_t i = 0; i < request.length(); i++) {
		if (request[i] == '\n' || i + 1 == request.length()) { // check si on arrive a la fin d'une ligne
			if ((request[i - 1] == '\r' && request[i - 2] == '\n') || i + 1 == request.length()) { //check si sur une ligne vide pour mettre le dernier char
				buffer.push_back(request[i]);
			} else if (buffer.find('\r') != std::string::npos) //check si \r et l'enleve si trouve
				buffer.erase(buffer.end() - 1);
			arr.push_back(buffer);
			buffer.clear();
		}
		else
			buffer.push_back(request[i]);
	}
	return arr;
};

std::vector<Token> tokenize(std::string request) {
	std::string buffer;
	std::vector<Token> tokens;
	Token token;

	std::vector<std::string> requestVec = splitRequest(request);
	bool isBody = false;
	int spaceCount = 0;
	for (size_t i = 0; i < requestVec.size(); i++) {
		if (i == 0) {
			for (size_t j = 0; j < requestVec[i].length(); j++) {
					if (requestVec[i][j] == ' ' || j + 1 == requestVec[i].length()) { //check si sur espace ou fin de string
						if (spaceCount == 0) {
							token._type = METHOD;
							token._value = buffer;
						} else if (spaceCount == 1) {
							token._type = PATH;
							token._value = buffer;
						} else {
							token._type = VERSION;
							buffer.push_back(requestVec[i][j]);
							token._value = buffer;
						}
						buffer.clear();
						spaceCount++;
						tokens.push_back(token);
					} else {
						buffer.push_back(requestVec[i][j]);
					}
			}
		} else {
			if (requestVec[i] == "\r\n") {
				isBody = true;
				continue;
			}
			if (isBody) {
				token._type = BODY;
				token._value = requestVec[i];
			} else {
				token._type = HEADERS;
				token._value = requestVec[i];
			}
			tokens.push_back(token);
		}
	}
	return tokens;
}