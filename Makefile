# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: croy <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/05 23:00:13 by croy              #+#    #+#              #
#    Updated: 2017/02/10 16:44:46 by nd-heyge         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = serveur

all: $(NAME)

$(NAME):
	make -C src/serveur/
	mv src/serveur/srv serveur

clean:
	make clean -C src/serveur/

fclean: clean
	rm -f serveur

re: fclean all
