# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/26 19:02:12 by tbalea            #+#    #+#              #
#    Updated: 2016/04/29 19:49:43 by tbalea           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = srv

SRC = ft_atoi.c\
	  ft_itoa.c\
	  ft_bzero.c\
	  ft_isempty.c\
	  ft_isdigit.c\
	  ft_strlen.c\
	  ft_strcmp.c\
	  ft_strdelelem.c\
	  ft_strdelvoid.c\
	  ft_strncmp.c\
	  ft_strnew.c\
	  ft_tabjoin.c\
	  ft_strsplit.c\
	  ft_strjoin.c\
	  ft_strcjoin.c\
	  ft_strtrim.c\
	  ft_memalloc.c\
	  ft_memchr.c\
	  ft_memcpy.c\
	  ft_memdel.c\
	  ft_memset.c\
	  ft_putchar.c\
	  ft_putstr.c\
	  ft_putendl.c\
	  ft_resizestr.c\
	  ft_tabdel.c\
	  ft_txtadd.c\
	  ft_tabdelelem.c\
	  ft_strcat.c\
	  ft_strncat.c\
	  ft_strdup.c\
	  server.c\
	  server_create.c\
	  server_client.c

OBJ = $(SRC:.c=.o)

FLG = -Wall -Werror -Wextra

GDB = -g3

CCC = gcc

RMF = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CCC) $(FLG) $(OBJ) -o $(NAME)

clean:
	$(RMF) $(OBJ)

fclean: clean
	$(RMF) $(NAME)

re: fclean all
