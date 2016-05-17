/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 16:18:55 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/17 17:43:35 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*mcl[] =
{
		"Player client from ip %s , port %d has disconnected.\n",
		"Reached client limit's.\n",
		"Player from ip %s, port %d tried to connect, but limit of %d client is\
 reached.\n",
		"New player from ip %s, port %d.\n"
};

t_client	*client_init(void)
{
	t_client	*clt;

	if (!(clt = (t_client *)malloc(sizeof(t_client)))
		|| !(clt->pos.rsc = (int *)malloc(8 * sizeof(int)))
		||	!(clt->ring = ring_init(10)))
	{
		clt != NULL && clt->ring != NULL ? free(clt->ring) : NULL;
		clt != NULL && clt->pos.rsc != NULL ? free(clt->pos.rsc) : NULL;
		clt != NULL ? free(clt) : NULL;
		return (clt);
	}
	clt->socket = 8;
	clt->pos.rsc[clt->socket - 1] = -1;
	while (--clt->socket > 0)
		clt->pos.rsc[clt->socket - 1] = 0;
	clt->len = sizeof(struct sockaddr_in);
	clt->next = NULL;
	clt->pos.x = 0;
	clt->pos.y = 0;
	clt->sens = 0;
	clt->team = 0;
	clt->lvl = 1;
	clt->time = 0.0f;
	clt->health = 1260.0f;
	return (clt);
}

void		client_kill(t_client *clt, t_fds *fds)
{
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	ring_kill(clt->ring);
	close(clt->socket);
	clt->socket = 9;
	while (--clt->socket > 0)
		clt->pos.rsc[clt->socket - 1] = 0;
	free(clt->pos.rsc);
	clt->pos.x = 0;
	clt->pos.y = 0;
	clt->sens = 0;
	clt->team = 0;
	clt->lvl = 0;
	clt->time = 0;
	clt->health = 0;
	free(clt);
	clt = NULL;
}

void		client_zero(t_client *clt, t_fds *fds)
{
	getpeername(clt->socket, (struct sockaddr*)&clt->sin, \
			(socklen_t*)&clt->len);
	printf(mcl[0], inet_ntoa(clt->sin.sin_addr), ntohs(clt->sin.sin_port));
	if (fds)
	{
		FD_CLR(clt->socket, &fds->rd);
		FD_CLR(clt->socket, &fds->wr);
		FD_CLR(clt->socket, &fds->ex);
	}
	ring_zero(clt->ring);
	close(clt->socket);
	clt->socket = 8;
	while (--clt->socket > 0)
		clt->pos.rsc[clt->socket - 1] = 0;
	clt->pos.x = 0;
	clt->pos.y = 0;
	clt->sens = 0;
	clt->lvl = 1;
	clt->team = 0;
	clt->time = 0;
	clt->health = 0;
}

//	TODO
//	*	Send to client result of connection
void		player_fork(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	t_client	*new;

	if (strncmp(cmd, "client", 6) != 0 || gfx->isgfx)
		return ;
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
