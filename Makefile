# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/25 13:28:44 by xin               #+#    #+#              #
#    Updated: 2026/01/17 13:57:49 by meyu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	RL_INC =
	RL_LIB = -lreadline
else ifeq ($(UNAME), Darwin)
	RL_PATH := /opt/homebrew/opt/readline
	RL_INC = -I$(RL_PATH)/include
	RL_LIB = -L$(RL_PATH)/lib -lreadline
endif

CFLAGS		= 	-Wall -Werror -Wextra $(RL_INC)

LIBFT		=	libft.a
LIB_DIR		=	lib/libft

GET_NEXT_LINE	=	get_next_line.a
GNL_DIR			=	lib/get_next_line

SRCS 		:=	src/main.c			\
				src/main_2.c		\
				src/main_3.c		\
				src/lexer.c			\
				src/lexer_2.c		\
				src/lexer_3.c		\
				src/lexer_4.c		\
				src/lexer_5.c		\
				src/token.c			\
				src/parser.c		\
				src/parser_2.c		\
				src/parser_3.c		\
				src/executor.c		\
				src/executor_2.c	\
				src/executor_3.c	\
				src/executor_4.c	\
				src/executor_5.c	\
				src/expander.c		\
				src/expander_2.c	\
				src/expander_3.c	\
				src/expander_4.c	\
				src/expander_5.c	\
				src/expander_6.c	\
				src/expander_7.c	\
				src/env.c			\
				src/env_2.c			\
				src/builtin.c		\
				src/builtin_2.c		\
				src/builtin_3.c		\
				src/builtin_4.c		\
				src/builtin_5.c		\
				src/signal.c		\
				src/heredoc.c		\
				src/heredoc_2.c		\
				src/free.c			\
				src/wildcard.c		\
				src/wildcard_2.c

OBJ := $(SRCS:src/%.c=build/minishell/%.o)

.PHONY: all build bonus clean fclean re

all: $(LIB_DIR)/$(LIBFT) $(GNL_DIR)/$(GET_NEXT_LINE) $(NAME) 

$(NAME): $(OBJ) $(LIB_DIR)/$(LIBFT) $(GNL_DIR)/$(GET_NEXT_LINE)
	cc $(CFLAGS) $(OBJ) $(LIB_DIR)/$(LIBFT) $(GNL_DIR)/$(GET_NEXT_LINE) $(RL_LIB) -o $(NAME)

build/minishell/%.o: src/%.c
	@mkdir -p $(dir $@)
	cc $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(LIBFT):
	make -C $(LIB_DIR)

$(GNL_DIR)/$(GET_NEXT_LINE):
	make -C $(GNL_DIR)

clean: 
	rm -rf build
	make -C $(LIB_DIR) clean
	make -C $(GNL_DIR) clean

fclean: clean
	rm -f $(NAME) $(CHECKER)
	make -C $(LIB_DIR) fclean
	make -C $(GNL_DIR) fclean

re: fclean all