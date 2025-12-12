# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xin <xin@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/10 14:57:06 by xin               #+#    #+#              #
#    Updated: 2025/12/10 14:57:42 by xin              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = get_next_line.a

CFLAGS 	= -Wall -Wextra -Werror -I../includes

SRCS	= 	get_next_line.c	\
			get_next_line_utils.c

OBJ		= $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)
$(NAME): $(OBJ)
	ar rcs $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

