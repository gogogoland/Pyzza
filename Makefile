# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: croy <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/05 23:00:13 by croy              #+#    #+#              #
#    Updated: 2017/02/06 01:06:17 by tbalea           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

all: $(NAME)

$(NAME):
	make -C src/serveur/
	mv src/serveur/srv .

clean:
	make clean -C src/serveur/

fclean: clean
	rm -f srv

re: fclean all
