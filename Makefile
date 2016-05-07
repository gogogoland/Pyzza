# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/04/30 17:45:52 by nd-heyge          #+#    #+#              #
#    Updated: 2016/05/07 16:07:16 by nd-heyge         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gfx

SRCC = main.cpp SDL.cpp Camera.cpp OpenGL.cpp Shader.cpp stb_image.cpp

DOBJ = obj/

OBJ = $(patsubst %.cpp, $(DOBJ)%.o, $(SRCC))
DEPS = $(patsubst %.cpp, $(DOBJ)%.d, $(SRCC))
DEPENDS = -MT $@ -MD -MP -MF $(subst .o,.d,$@)

FLAG = -g -Wall -Werror -Wextra -std=c++11

FRAMEWORK = -framework OpenGL

LIB = -L$(HOME)/.brew/lib -lSDL2 -lSDL2_image

all:		$(NAME)

$(NAME):	$(OBJ)
	g++ $(FLAG) -o $@ $^ $(LIB) $(FRAMEWORK)

-include	$(OBJ:.o=.d)

$(DOBJ)%.o:	%.cpp
	mkdir -p $(DOBJ)
	mkdir -p $(DOBJ)srcs
	g++ $(FLAG) $(DEPENDS) -c $< -o $@ -I$(HOME)/.brew/include

clean:
	/bin/rm -rf $(DOBJ)

fclean:		clean
	/bin/rm -f $(NAME)
	/bin/rm -rf $(NAME).dSYM

re:			fclean all

.PHONY:		all re fclean clean
