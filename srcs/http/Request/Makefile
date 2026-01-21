# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# Executable name
NAME = tokenize

# Source files
SRCS = mainRequest.cpp tokenizer.cpp checker.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(NAME)

# Link executable
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Compile object files
%.o: %.cpp request.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

# Clean everything
fclean: clean
	rm -f $(NAME)

# Rebuild
re: fclean all

.PHONY: all clean fclean re
