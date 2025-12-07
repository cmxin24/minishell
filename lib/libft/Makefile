# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xin <xin@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/10 18:45:41 by meyu              #+#    #+#              #
#    Updated: 2025/12/07 15:49:21 by xin              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
SOURCE =		ft_calloc.c		\
	 			ft_isdigit.c	\
				ft_memcmp.c		\
				ft_putchar_fd.c	\
				ft_split.c		\
				ft_strjoin.c	\
				ft_strmapi.c	\
				ft_strtrim.c	\
				ft_isalnum.c	\
				ft_isprint.c	\
				ft_memcpy.c		\
				ft_putendl_fd.c	\
				ft_strchr.c		\
				ft_strlcat.c	\
				ft_strcmp.c		\
				ft_strncmp.c	\
				ft_substr.c		\
				ft_atoi.c		\
				ft_isalpha.c	\
				ft_itoa.c		\
				ft_memmove.c	\
				ft_putnbr_fd.c	\
				ft_strdup.c		\
				ft_strlcpy.c	\
				ft_strnstr.c	\
				ft_tolower.c	\
				ft_bzero.c		\
				ft_isascii.c	\
				ft_memchr.c		\
				ft_memset.c		\
				ft_putstr_fd.c	\
				ft_striteri.c	\
				ft_strlen.c		\
				ft_strrchr.c	\
				ft_toupper.c
BONUS_SOURCE =	ft_lstnew.c			\
				ft_lstadd_front.c	\
				ft_lstsize.c		\
				ft_lstlast.c		\
				ft_lstadd_back.c	\
				ft_lstdelone.c		\
				ft_lstclear.c		\
				ft_lstiter.c		\
				ft_lstmap.c
CFLAGS = -Wall -Wextra -Werror -I../includes
OBJECT = $(SOURCE:.c=.o)
BONUS_OBJECT = $(BONUS_SOURCE:.c=.o)

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJECT) $(BONUS_OBJECT)
	ar rcs $(NAME) $(OBJECT) $(BONUS_OBJECT)

clean:
	rm -f $(OBJECT) $(BONUS_OBJECT)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: $(OBJECT) $(BONUS_OBJECT)
	ar rcs $(NAME) $(OBJECT) $(BONUS_OBJECT)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@