/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:39:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/04 16:48:09 by tbalea           ###   ########.fr       */
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

typedef struct			s_ring
{
	char				**command;
	int					cur;
	int					len;
}						t_ring;

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

typedef struct			s_tmp
{
	int					socket;
	unsigned int        len;
	struct sockaddr_in  sin;
}						t_tmp;

typedef struct			s_gfx
{
	int					socket;
	unsigned int        len;
	struct sockaddr_in  sin;
	struct s_gfx		*prev;
	struct s_gfx		*next;
}						t_gfx;

typedef struct			s_client
{
	int					socket;
	unsigned int        len;
	struct sockaddr_in  sin;
	struct s_coord		pos;
	struct s_ring		*ring;
	struct s_client		*next;
	struct s_client		*prev;
}						t_client;

typedef struct			s_server
{
	struct s_coord		plateau;
	int					port;
	int					socket;
	int					time;
	int					old_player_max;
	int					player_max;
	char				**team;
	struct s_gfx		*gfx;
	struct s_client		*clt;
}						t_server;

void		ring_recv(char *command, t_ring *ring);
char		*ring_send(t_ring *ring);
t_ring		*ring_init(int len);
void		ring_zero(t_ring *ring);
void		ring_kill(t_ring *ring);

t_client	*client_init(void);
void		client_zero(t_client *clt, t_fds *fsd);
void		client_kill(t_client *clt, t_fds *fsd);

t_gfx		*graphe_init(t_gfx *prev, t_tmp tmp);
void		graphe_kill(t_gfx *gfx, t_fds *fsd);

t_server	*server_create(int argc, char **argv);
bool		recv_client(t_fds *fds, t_server *srv, int ret);

#endif
