NAME        = webserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I./includes

SRCS_DIR    = srcs
OBJS_DIR    = objs

SRCS        = srcs/main.c++ \
              srcs/config/ConfigParser.cpp \
              srcs/config/Server.cpp \
              srcs/config/Location.cpp \
              srcs/config/Config.cpp

OBJS        = $(patsubst $(SRCS_DIR)/%.c++,$(OBJS_DIR)/%.o,$(filter %.c++,$(SRCS))) \
              $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(filter %.cpp,$(SRCS)))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c++
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re