/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_graphe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 16:33:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 21:28:01 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_gfx[] =
{
	"New graphical client from ip %s, port %d.\n",
	"pnw #%d %d %d %d %d %d\n",
	"enw #%d %d %d %d\n"
};

static void	command_graphe_client_co(int socket, t_client *clt, int type)
{
	char	*msg;

	msg = NULL;
	if (type)
	{
		asprintf(&msg, g_cmd_gfx[1], clt->socket, clt->pos.x, clt->pos.y,
				clt->sens, clt->lvl, clt->team);
	}
	else
	{
		asprintf(&msg, g_cmd_gfx[2], clt->socket, clt->pos.x, clt->pos.y,
				clt->team);
	}
	if (msg)
	{
		send(socket, msg, strlen(msg), 0);
		free(msg);
		msg = NULL;
	}
}

void		command_graphe(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	t_client	*clt;

	gfx->isgfx = true;
	printf(g_cmd_gfx[0],
			inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port));
	clt = srv->clt;
	command_size(fds, srv, gfx, cmd);
	//command_team(fds, srv, gfx, cmd);
	while (clt)
	{
		if (clt->health > 0.0f || clt->time > 0.0f)
			command_graphe_client_co(gfx->socket, clt, !clt->socket ? 0 : 1);
		clt = clt->next;
	}
	command_map(fds, srv, gfx, cmd);
}
