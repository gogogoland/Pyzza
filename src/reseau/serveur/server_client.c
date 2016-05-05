/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 16:18:55 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/05 19:58:26 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*mcl[] =
{
		"Client from ip %s , port %d disconnect.\n",
		"Reached client limit's.\n",
		"New player from ip %s, port %d.\n",
		"Player from ip %s, port %d tried to connect, but limit of %d client is\
 reached.\n"
};

t_client	*client_init(void)
{
	t_client	*clt;

	if (!(clt = (t_client *)malloc(sizeof(t_client))))
		return (clt);
	clt->socket = 0;
	clt->len = sizeof(struct sockaddr_in);
	clt->ring = ring_init(10);
	clt->next = NULL;
	clt->pos.x = 0;
	clt->pos.y = 0;
	return (clt);
}

void		client_kill(t_client *clt, t_fds *fds)
{
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	ring_kill(clt->ring);
	close(clt->socket);
	clt->socket = 0;
	clt->pos.x = 0;
	clt->pos.y = 0;
	free(clt);
	clt = NULL;
}

void		client_zero(t_client *clt, t_fds *fds)
{
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
}

//	TODO
//	*	Send to client result of connection
void		player_fork(t_fds *fds, t_server *srv, t_gfx *gfx)
{
	t_client	*new;

	new = srv->clt;
	while (new && new->socket != 0)
		new = new->next;
	if (!new)
	{
		printf(mcl[2], inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port), \
				srv->player_max);
		graphe_kill(gfx, fds, false);
	}
	else
	{
		new->socket = gfx->socket;
		new->sin = gfx->sin;
		new->len = gfx->len;
		printf(mcl[3], inet_ntoa(new->sin.sin_addr) , ntohs(new->sin.sin_port));
		fds->max = new->socket > fds->max-1 ? new->socket+1 : fds->max;
		graphe_kill(gfx, fds, true);
	}
}
