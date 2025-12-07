# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xin <xin@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/25 13:28:44 by xin               #+#    #+#              #
#    Updated: 2025/12/07 15:51:46 by xin              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

CFLAGS		= 	-Wall -Werror -Wextra -Iincludes

LIBFT		=	libft.a
LIB_DIR		=	lib/libft

SRCS 		:=	src/main.c			\
				src/lexer.c			\
				src/lexer_2.c		\
				src/token.c			\
				src/parser.c		\
				src/executor.c		\
				src/expander.c		\
				src/expander_2.c	\
				src/env.c			\
				src/env_2.c			\
				src/builtin.c		\
				src/builtin_2.c		\
				src/free.c

OBJ := $(SRCS:src/%.c=build/minishell/%.o)

.PHONY: all build bonus clean fclean re

all: $(LIB_DIR)/$(LIBFT) $(NAME)

$(NAME): $(OBJ) $(LIB_DIR)/$(LIBFT)
	cc $(CFLAGS) -lreadline $^ -o $@
	
build/minishell/%.o: src/%.c
	@mkdir -p $(dir $@)
	cc $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(LIBFT):
	make -C $(LIB_DIR)

clean: 
	rm -rf build
	make -C $(LIB_DIR) clean

fclean: clean
	rm -f $(NAME) $(CHECKER)
	make -C $(LIB_DIR) fclean

re: fclean all