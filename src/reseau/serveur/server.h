/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:39:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/27 21:03:34 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef server_H
# define SERVER_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/dir.h>
# include <sys/stat.h>
# include <netdb.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <arpa/inet.h>

typedef struct sockaddr_in  t_sokadr_in;
typedef struct sockaddr     t_sokadr;
typedef struct in_addr      t_i_adr;

typedef struct			s_plateau
{
	int					size_x;
	int					size_y;
}						t_plateau;

typedef struct			s_fds
{
	fd_set				rd_fds;
	fd_set				wr_fds;
	fd_set				ex_fds;
}						t_fds;

typedef struct			s_server
{
	struct s_plateau	plateau;
	int					port;
	int					socket;
	int					time;
	int					old_player_max;
	int					player_max;
	char				**team;
}						t_server;

t_server				*server_create(int argc, char **argv);

#endif
