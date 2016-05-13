/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:39:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/12 18:45:27 by tbalea           ###   ########.fr       */
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
	int					*rsc;
}						t_coord;

typedef struct			s_fds
{
	fd_set				rd;
	fd_set				wr;
	fd_set				ex;
	int					max;
}						t_fds;

typedef struct			s_gfx
{
	int					socket;
	unsigned int        len;
	struct sockaddr_in  sin;
	struct s_ring		*ring;
	struct s_gfx		*prev;
	struct s_gfx		*next;
	bool				isgfx;
}						t_gfx;

typedef struct			s_client
{
	int					socket;
	unsigned int        len;
	struct sockaddr_in  sin;
	struct s_coord		pos;
	int					sens;
	int					team;
	bool				fork;
	int					*rsc;
	int					lvl;
	int					vie;
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
	int					***map;
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
void		player_fork(t_fds *fsd, t_server *srv, t_gfx *gfx, char *cmd);
void		client_zero(t_client *clt, t_fds *fsd);
void		client_kill(t_client *clt, t_fds *fsd);

t_gfx		*graphe_init(t_gfx *prev, t_fds *fds, int s);
void		graphe_kill(t_gfx *gfx, t_fds *fsd, bool gfxtoclt);

void	    init_map(t_server *srv);
void	    kill_map(t_server *srv);

t_server	*server_create(int argc, char **argv);

bool		recv_client(t_fds *fds, t_server *srv, int ret);
bool		send_client(t_fds *fds, t_server *srv, int ret);
void		send_client_data(t_client *clt);
void		send_graphe_action(t_server *srv, t_client *clt, int n);

void		command_forward(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_death(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_eject(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_forward(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_inventory(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_left(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_lvlup(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_msg(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_pose(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_right(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_seek(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);
void		command_take(t_fds *fds, t_server *srv,
							t_client *clt, char *cmd);

void		command_graphe(t_fds *fds, t_server *srv,
							t_gfx *gfx, char *cmd);
void		command_map(t_fds *fds, t_server *srv,
							t_gfx *gfx, char *cmd);
//void		command_(t_fds *fds, t_server *srv, t_ *, char *cmd);

#endif