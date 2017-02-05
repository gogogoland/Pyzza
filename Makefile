# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: croy <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/02/05 23:00:13 by croy              #+#    #+#              #
#    Updated: 2017/02/05 23:04:36 by croy             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

all: $(NAME)

$(NAME):
	make -C src/serveur/

clean:
	make clean -C src/serveur/

fclean:
	make fclean -C src/serveur/

re: all fclean
