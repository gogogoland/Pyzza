/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 16:18:55 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/22 20:13:36 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_mcl[] =
{
	"Player client from ip %s , port %d has disconnected.\n",
	"Reached client limit's.\n",
};

void	client_init_data(t_client *clt)
{
	clt->action = 0;
	clt->pos.x = 0;
	clt->pos.y = 0;
	clt->sens = 0;
	clt->team = -1;
	clt->lvl = 1;
	clt->tolvl = 0;
	clt->time = 0.0f;
	clt->health = 0.0f;
	clt->broadcast = NULL;
	clt->acolyte = NULL;
	clt->casting = 0;
}

t_client	*client_init(t_server *srv)
{
	t_client	*clt;

	if (!(clt = (t_client *)malloc(sizeof(t_client)))
		|| !(clt->pos.rsc = (int *)malloc(8 * sizeof(int)))
		|| !(clt->ring = ring_init(10)))
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
	client_init_data(clt);
	clt->name = ++srv->name;
	return (clt);
}

void	client_kill(t_client *clt, t_fds *fds)
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
	clt->lvl = 0;
	if (clt->broadcast)
		free(clt->broadcast);
	if (clt->acolyte)
		free(clt->acolyte);
	client_init_data(clt);
	free(clt);
	clt = NULL;
}

void	client_zero(t_client *clt, t_fds *fds)
{
	getpeername(clt->socket, (struct sockaddr*)&clt->sin,
			(socklen_t*)&clt->len);
	printf(g_mcl[0], inet_ntoa(clt->sin.sin_addr), ntohs(clt->sin.sin_port));
	if (fds)
	{
		FD_CLR(clt->socket, &fds->rd);
		FD_CLR(clt->socket, &fds->wr);
		FD_CLR(clt->socket, &fds->ex);
	}
	if (clt->broadcast)
		free(clt->broadcast);
	if (clt->acolyte)
		free(clt->acolyte);
	ring_zero(clt->ring);
	close(clt->socket);
	clt->socket = 8;
	while (--clt->socket > 0)
		clt->pos.rsc[clt->socket - 1] = 0;
	client_init_data(clt);
}
