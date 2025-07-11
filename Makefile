# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mapham <mapham@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/07 05:02:16 by mapham            #+#    #+#              #
#    Updated: 2025/07/07 09:54:19 by mapham           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC_DIR = srcs
INCLUDE = includes

SRC = $(SRC_DIR)/init.c \
      $(SRC_DIR)/philo.c \
	  $(SRC_DIR)/parser.c \
	  $(SRC_DIR)/monitoring.c \
	  $(SRC_DIR)/eating.c \
	  $(SRC_DIR)/philo_tools.c \
      $(SRC_DIR)/utils.c \
	  main.c

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g3
FLAGS = -I$(INCLUDE)

.c.o:
	@$(CC) $(CFLAGS) $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
