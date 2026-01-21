#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Request/tokenizer.hpp"

#define OK 200
#define BadRequest 400
#define NotFound 404
#define MethodNotAllowed 405
#define WrongVersion 505

int checkRequest(std::vector<Token> tokens, std::map<std::string, std::string> headers);
std::map<std::string, std::string> mapHeaders(std::vector<Token> tokens);