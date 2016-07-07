/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/06 20:55:34 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/07 22:12:10 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_plr[] =
{
	"Player from ip %s, port %d tried to connect, but limit of %d client is\
reached.\n",
	"New player #%d from ip %s, port %d.\n",
	"suc\n"
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

static bool		command_player_is_graphical(t_gfx *gfx)
{
	if (!gfx->isgfx)
		return (false);
	send(gfx->socket, g_cmd_plr[2], strlen(g_cmd_plr[2]), 0);
	return (true);
}

static void		command_player_send_welcome(t_server *srv, t_client *clt)
{
	t_client	*team;
	char		*msg;
	int			nb_clt;

	msg = NULL;
	team = srv->clt;
	nb_clt = 0;
	while (team)
	{
		if (team->socket == 0 && (team->team == clt->team || team->team == -1))
			nb_clt++;
		team = team->next;
	}
	asprintf(&msg, "%i\n%i %i\n", nb_clt, clt->pos.x, clt->pos.y);
	send(clt->socket, msg, strlen(msg), 0);
	ft_memdel((void **)&msg);
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

	if (!srv->team[(t = command_player_get_team(srv, cmd))]
			|| command_player_is_graphical(gfx))
		return ;
	if (!(new = command_player_get_valide_client(t, srv)))
	{
		printf(g_cmd_plr[0], inet_ntoa(gfx->sin.sin_addr),
				ntohs(gfx->sin.sin_port), srv->player_max);
		graphe_kill(gfx, fds, false);
		return ;
	}
	client_init_data(new);
	new->socket = gfx->socket;
	new->sin = gfx->sin;
	new->len = gfx->len;
	new->team = t;
	new->health = 1260.0f / (float)srv->time;
	printf(g_cmd_plr[1], new->name, inet_ntoa(new->sin.sin_addr),
			ntohs(new->sin.sin_port));
	fds->max = new->socket > fds->max - 1 ? new->socket + 1 : fds->max;
	graphe_kill(gfx, fds, true);
	command_player_send_welcome(srv, new);
}
