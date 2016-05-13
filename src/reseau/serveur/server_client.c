/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 16:18:55 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/12 18:48:20 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*mcl[] =
{
		"Client from ip %s , port %d disconnect.\n",
		"Reached client limit's.\n",
		"Player from ip %s, port %d tried to connect, but limit of %d client is\
 reached.\n",
		"New player from ip %s, port %d.\n"
};

t_client	*client_init(void)
{
	t_client	*clt;

ft_putendl("server_client.c	client_init	0");
	if (!(clt = (t_client *)malloc(sizeof(t_client))))
		return (clt);
ft_putendl("server_client.c	client_init	1");
	clt->socket = 0;
	clt->len = sizeof(struct sockaddr_in);
	clt->ring = ring_init(10);
	clt->next = NULL;
	clt->pos.x = 0;
	clt->pos.y = 0;
	clt->sens = 0;
	clt->team = 0;
ft_putendl("server_client.c	client_init	2");
	return (clt);
}

void		client_kill(t_client *clt, t_fds *fds)
{
ft_putendl("server_client.c	client_kill	0");
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	ring_kill(clt->ring);
	close(clt->socket);
	clt->socket = 0;
	clt->pos.x = 0;
	clt->pos.y = 0;
	clt->sens = 0;
	clt->team = 0;
	free(clt);
	clt = NULL;
ft_putendl("server_client.c	client_kill	1");
}

void		client_zero(t_client *clt, t_fds *fds)
{
ft_putendl("server_client.c	client_zero	0");
	getpeername(clt->socket, (struct sockaddr*)&clt->sin, \
			(socklen_t*)&clt->len);
	printf(mcl[0], inet_ntoa(clt->sin.sin_addr) , ntohs(clt->sin.sin_port));
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	ring_zero(clt->ring);
	close(clt->socket);
	clt->socket = 0;
	clt->pos.x = 0;
	clt->pos.y = 0;
	clt->sens = 0;
	clt->team = 0;
ft_putendl("server_client.c	client_zero	1");
}

//	TODO
//	*	Send to client result of connection
void		player_fork(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	t_client	*new;

ft_putendl("server_client.c	player_fork	0");
	if (strncmp(cmd, "client", 6) != 0)
		return ;
ft_putendl("server_client.c	player_fork	1");
	new = srv->clt;
	while (new && new->socket != 0)
		new = new->next;
ft_putendl("server_client.c	player_fork	2");
	if (!new)
	{
ft_putendl("server_client.c	player_fork	3");
		printf(mcl[2], inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port), \
				srv->player_max);
ft_putendl("server_client.c	player_fork	4");
		graphe_kill(gfx, fds, false);
ft_putendl("server_client.c	player_fork	5");
	}
	else
	{
ft_putendl("server_client.c	player_fork	6");
		new->socket = gfx->socket;
		new->sin = gfx->sin;
		new->len = gfx->len;
		printf(mcl[3], inet_ntoa(new->sin.sin_addr) , ntohs(new->sin.sin_port));
		fds->max = new->socket > fds->max-1 ? new->socket+1 : fds->max;
ft_putendl("server_client.c	player_fork	7");
		graphe_kill(gfx, fds, true);
ft_putendl("server_client.c	player_fork	8");
	}
ft_putendl("server_client.c	player_fork	9");
}
