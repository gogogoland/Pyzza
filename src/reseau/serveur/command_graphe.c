/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_graphe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 16:33:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/21 22:31:56 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_gfx[] =
{
	"New graphical client from ip %s, port %d.\nSocket %i is now graphic.\n",
	"pnw #%d %d %d %d %d %d\n",
	"enw #%d #%d %d %d\n",
	"tna %s\n"
};

static void	command_graphe_log(t_server *srv, t_gfx *gfx, int type)
{
	char	*log;

	if ((!type && asprintf(&log, g_cmd_gfx[0], inet_ntoa(gfx->sin.sin_addr),
					ntohs(gfx->sin.sin_port), gfx->socket)))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
}

static void	command_graphe_client_co(int socket, t_client *clt, int type)
{
	char	*msg;

	msg = NULL;
	if (type)
	{
		asprintf(&msg, g_cmd_gfx[1], clt->name, clt->pos.x, clt->pos.y,
				clt->sens, clt->lvl, clt->team);
	}
	else
	{
		asprintf(&msg, g_cmd_gfx[2], clt->name, clt->pos.x, clt->pos.y,
				clt->team);
	}
	if (msg)
	{
		send(socket, msg, strlen(msg), 0);
		free(msg);
		msg = NULL;
	}
}

static void	command_graphe_send_team(t_server *srv, t_gfx *gfx)
{
	char		*team;
	int			i;

	i = -1;
	team = NULL;
	while (srv && srv->team && srv->team[++i])
	{
		if (asprintf(&team, g_cmd_gfx[3], srv->team[i]))
		{
			send(gfx->socket, team, ft_strlen(team), 0);
			ft_memdel((void **)&team);
		}
	}
}

void		command_graphe(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	t_client	*clt;

	if (gfx->isgfx)
		return ;
	gfx->isgfx = true;
	command_graphe_log(srv, gfx, 0);
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
	command_time_server(fds, srv, gfx, cmd);
	command_graphe_send_team(srv, gfx);
}
