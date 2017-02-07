# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/26 19:02:12 by tbalea            #+#    #+#              #
#    Updated: 2017/02/07 18:01:03 by tbalea           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = srv

SRC = ft_atoi.c\
	  ft_itoa.c\
	  ft_bzero.c\
	  ft_isempty.c\
	  ft_isdigit.c\
	  ft_strisdigit.c\
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
	  ft_strbdup.c\
	  server.c\
	  parser.c\
	  parser_int.c\
	  parser_team.c\
	  server_create.c\
	  server_client.c\
	  server_graphe.c\
	  server_time.c\
	  server_send.c\
	  server_recv.c\
	  server_ring.c\
	  server_ring_recv.c\
	  server_map.c\
	  server_log.c\
	  save_cur_cmd.c\
	  command_death.c\
	  command_eat.c\
	  command_eject.c\
	  command_forward.c\
	  command_inventory.c\
	  command_left.c\
	  command_msg.c\
	  command_pose.c\
	  command_right.c\
	  command_seek.c\
	  command_take.c\
	  command_fork.c\
	  command_incant.c\
	  incant_help_acolyte.c\
	  incant_help_acolyte_msg.c\
	  command_nbr_co.c\
	  command_player.c\
	  command_graphe.c\
	  command_size.c\
	  command_write_msg.c\
	  command_get_int.c\
	  command_time_change.c\
	  command_time_server.c\
	  command_map.c\
	  command_box.c\
	  command_box_content.c\
	  command_player_inv.c\
	  command_player_lvl.c\
	  command_player_pos.c\
	  command_team_name.c\
	  command_graphical_bad_parameters.c\
	  generate_map.c

OBJ = $(SRC:.c=.o)

FLG = -Wall -Werror -Wextra

GDB = -g

CCC = gcc

RMF = rm -f

all: $(NAME)

%.o:%.c
	$(CCC) $(FLG) -c $< -o $@

$(NAME): $(OBJ)
	$(CCC) $(FLG) $(OBJ) $(GDB) -o $(NAME)

clean:
	$(RMF) $(OBJ)

fclean: clean
	$(RMF) $(NAME)

re: fclean all
