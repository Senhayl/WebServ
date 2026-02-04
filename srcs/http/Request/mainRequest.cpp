#include "../httpHeader.hpp"

int main() {

	std::cout << getAnswer("DELETE /test.html HTTP/1.1\r\nHost: test.com") << std::endl;
	
	return 0;
}