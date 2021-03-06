/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 16:18:55 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 18:53:45 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		client_init_data(t_client *clt, t_server *srv)
{
	srand(time(NULL));
	clt->mom = 0;
	clt->action = 0;
	clt->pos.x = srv ? rand() % srv->plateau.x : 0;
	clt->pos.y = srv ? rand() % srv->plateau.y : 0;
	clt->sens = rand() % 4;
	clt->team = -1;
	clt->lvl = 1;
	clt->tolvl = 0;
	clt->time = 0;
	clt->health = 0;
	clt->current_cmd = NULL;
	clt->fornext_cmd = NULL;
	clt->acolyte = NULL;
	clt->casting = 0;
	clt->fork = false;
}

t_client	*client_init(t_server *srv)
{
	t_client	*clt;

	if (!(clt = (t_client *)malloc(sizeof(t_client)))
		|| !(clt->pos.rsc = (int *)malloc(8 * sizeof(int)))
		|| !(clt->ring = ring_init(srv, 10)))
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
	clt->name = srv ? ++srv->name : 0;
	client_init_data(clt, srv);
	return (clt);
}

void		client_kill(t_client *clt, t_fds *fds)
{
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	ring_kill(clt->ring);
	if (clt->socket > 0)
		close(clt->socket);
	clt->socket = 9;
	while (--clt->socket > 0)
		clt->pos.rsc[clt->socket - 1] = 0;
	free(clt->pos.rsc);
	clt->lvl = 0;
	if (clt->current_cmd)
		free(clt->current_cmd);
	if (clt->fornext_cmd)
		free(clt->fornext_cmd);
	if (clt->acolyte)
		free(clt->acolyte);
	client_init_data(clt, NULL);
	free(clt);
	clt = NULL;
}

void		client_zero(t_client *clt, t_fds *fds, t_server *srv)
{
	if (fds)
	{
		FD_CLR(clt->socket, &fds->rd);
		FD_CLR(clt->socket, &fds->wr);
		FD_CLR(clt->socket, &fds->ex);
	}
	if (clt->current_cmd)
		free(clt->current_cmd);
	if (clt->fornext_cmd)
		free(clt->fornext_cmd);
	if (clt->acolyte)
		free(clt->acolyte);
	ring_zero(clt->ring);
	if (clt->socket > 0)
		close(clt->socket);
	clt->socket = 8;
	while (--clt->socket > 0)
		clt->pos.rsc[clt->socket - 1] = 0;
	client_init_data(clt, srv);
}
