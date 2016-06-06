/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/06 20:55:34 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/06 21:22:11 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_plr[] =
{
	"Player from ip %s, port %d tried to connect, but limit of %d client is\
reached.\n",
	"New player from ip %s, port %d.\n"
};

static int		command_player_get_team(t_server *srv, char *cmd)
{
	int	t;

	t = 0;
	while (srv->team[t] && strncmp(srv->team[t], cmd, strlen(cmd) - 1) != 0)
		t++;
	return (t);
}

static t_client	*command_player_get_valide_client(int t, t_server *srv)
{
	t_client	*new;

	new = srv->clt;
	while (new && (new->socket || (new->team >= 0 && new->team != t)))
		new = new->next;
	return (new);
}

//	TODO
//	*	Send to client
//	*	*	<nb-client>\n
//	*	*	<x> <y>\n
void			command_player(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			t;
	char		*team;
	t_client	*new;

	if (!srv->team[(t = command_player_get_team(srv, cmd))] || gfx->isgfx)
		return ;
	if (!(new = command_player_get_valide_client(t, srv)))
	{
		printf(g_cmd_plr[0], inet_ntoa(gfx->sin.sin_addr),
				ntohs(gfx->sin.sin_port), srv->player_max);
		graphe_kill(gfx, fds, false);
	}
	else
	{
		client_init_data(new);
		new->socket = gfx->socket;
		new->sin = gfx->sin;
		new->len = gfx->len;
		new->team = t;
		new->health = 1260.0f / (float)srv->time;
		printf(g_cmd_plr[1], inet_ntoa(new->sin.sin_addr),
				ntohs(new->sin.sin_port));
		fds->max = new->socket > fds->max - 1 ? new->socket + 1 : fds->max;
		graphe_kill(gfx, fds, true);
	}
}
