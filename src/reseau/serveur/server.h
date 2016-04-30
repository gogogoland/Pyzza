/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:39:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/30 16:54:48 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef server_H
# define SERVER_H

typedef char bool;
# define true 1
# define false 0

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

typedef struct			s_coord
{
	int					x;
	int					y;
}						t_coord;

typedef struct			s_fds
{
	fd_set				rd;
	fd_set				wr;
	fd_set				ex;
	int					max;
}						t_fds;

typedef struct			s_server
{
	struct s_coord		plateau;
	int					port;
	int					socket;
	int					time;
	int					old_player_max;
	int					player_max;
	char				**team;
}						t_server;

typedef struct			s_client
{
	int					socket;
	unsigned int        len;
	struct sockaddr_in  sin;
	struct s_coord		pos;
	struct s_client		*next;
	struct s_client		*prev;
}						t_client;

t_server	*server_create(int argc, char **argv);
void		client_connect(int s, t_client *clt, t_fds *fds, t_server *srv);
void		client_command(int s, t_client *clt, t_fds *fds, t_server *srv);

#endif
