# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/31 11:28:04 by aaiache           #+#    #+#              #
#    Updated: 2026/01/21 14:52:35 by aaiache          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN = \033[0;32m
CYAN = \033[0;36m
YELLOW = \033[0;33m

NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

ALL_SRCS = main.cpp Server.cpp
HEADERS = Server.hpp
BUILD_DIR = objs
OBJS = $(ALL_SRCS:%.cpp=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@echo "$(GREEN)-> Compilation de $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)
	@:

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)-- Compilation terminée avec succès. --\n"
	@echo "$(CYAN)-> Executable $(NAME) créé.\n"

clean:
	@echo "$(YELLOW)Suppression des fichiers objets"
	@$(RM) -r $(BUILD_DIR)

fclean: clean
	@echo "$(YELLOW)Suppression de l'exécutable $(NAME)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re