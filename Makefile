# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/21 15:57:46 by aaiache           #+#    #+#              #
#    Updated: 2026/01/28 17:58:28 by aaiache          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m

NAME        = webserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I./includes

SRCS_DIR    = srcs
OBJS_DIR    = objs

SRCS        = srcs/main.c++ \
              srcs/config/ConfigParser.cpp \
              srcs/config/Server.cpp \
              srcs/config/Location.cpp \
              srcs/config/Config.cpp \
			  srcs/server/Client.cpp \
			  srcs/server/Loop.cpp \
			  srcs/server/Server.cpp

HEADERS		= srcs/config/ConfigParser.hpp \
              srcs/config/Server.hpp \
              srcs/config/Location.hpp \
              srcs/config/Config.hpp \
			  srcs/server/Client.hpp \
			  srcs/server/Loop.hpp \
			  srcs/server/Server.hpp

OBJS        = $(patsubst $(SRCS_DIR)/%.c++,$(OBJS_DIR)/%.o,$(filter %.c++,$(SRCS))) \
              $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(filter %.cpp,$(SRCS)))

all: $(NAME)
	@:

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)-- Compilation terminée avec succès. --\n"
	@echo "$(CYAN)-> Executable $(NAME) créé.\n"

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c++ $(HEADERS)
	@mkdir -p $(dir $@)
	@echo "$(GREEN)-> Compilation de $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@echo "$(GREEN)-> Compilation de $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)Suppression des fichiers objets"
	@rm -rf $(OBJS_DIR)

fclean: clean
	@echo "$(YELLOW)Suppression de l'exécutable $(NAME)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re