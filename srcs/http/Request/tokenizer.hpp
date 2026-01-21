#pragma once

#include <iostream>
#include <string>
#include <vector>

enum TokenType {
	METHOD,
	VERSION,
	HEADERS,
	PATH,
	BODY
};

struct Token {
	TokenType _type;
	std::string _value;
};

std::vector<Token> tokenize(std::string request);