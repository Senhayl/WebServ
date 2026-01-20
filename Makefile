# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaiache <aaiache@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/31 11:28:04 by aaiache           #+#    #+#              #
#    Updated: 2025/12/31 11:28:35 by aaiache          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

ALL_SRCS = main.cpp
BUILD_DIR = objs
OBJS = $(ALL_SRCS:%.cpp=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "\033[0;32m-> Compilation de $<"
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)
	@echo "\033[0;32m\n----   Compilation terminée avec succès.   ----"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	@echo "\033[0;33mSuppression des fichiers objets"
	@$(RM) -r $(BUILD_DIR)

fclean: clean
	@echo "\033[0;33mSuppression de l'exécutable $(NAME)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re