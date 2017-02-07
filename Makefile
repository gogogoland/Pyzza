# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: croy <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/05 23:00:13 by croy              #+#    #+#              #
#    Updated: 2017/02/07 22:30:31 by croy             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

all: $(NAME)

$(NAME):
	make -C src/serveur/
	mv src/serveur/srv serveur

clean:
	make clean -C src/serveur/

fclean: clean
	rm -f serveur

re: fclean all
