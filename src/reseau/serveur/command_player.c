/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/06 20:55:34 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/22 14:40:30 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_plr[] =
{
	"Client from ip %s, port %d tried to connect, but limit of %d player is \
	reached.\n",
	"New player #%d from ip %s, port %d.\nSocket %i is now player\n",
	"suc\n"
};

static void		command_player_log(t_server *srv, int type, t_client *clt,
									t_gfx *gfx)
{
	char	*log;

	if ((type == 0 && asprintf(&log, g_cmd_plr[type],
				inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port),
				srv->player_max - srv->egg))
			|| (type == 1 && asprintf(&log, g_cmd_plr[type], clt->name,
				inet_ntoa(clt->sin.sin_addr), ntohs(clt->sin.sin_port),
				clt->socket))
			|| (type == 2 && asprintf(&log, "%s", g_cmd_plr[type])))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
}

static int		command_player_get_team(t_server *srv, char *cmd)
{
	int	t;

	t = 0;
	if (!cmd)
		return (-2);
	while (srv->team[t] && strncmp(srv->team[t], cmd, strlen(srv->team[t]))
			&& strncmp(srv->team[t], cmd, strlen(srv->team[t])))
		t++;
	return (t);
}

static t_client	*command_player_get_valide_client(int t, t_server *srv)
{
	t_client	*new;

	new = srv->clt;
	while (new && (new->socket || new->time > 0.0f
					|| (new->team >= 0 && new->team != t)))
		new = new->next;
	if (new && new->team == t)
		send_graphe_action(srv, command_write_msg(new, 0, 0, NULL), 0, NULL);
	return (new);
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
	send_graphe_action(srv, command_write_msg(clt, 8, 0, NULL), 0, NULL);
	ft_memdel((void **)&msg);
}

void			command_player(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			t;
	char		*team;
	t_client	*new;

	gfx->isgfx ? send(gfx->socket, g_cmd_plr[2], strlen(g_cmd_plr[2]), 0) : 0;
	if (gfx->isgfx || !srv->team[(t = command_player_get_team(srv, cmd))])
		return ;
	if (!(new = command_player_get_valide_client(t, srv)))
	{
		command_player_log(srv, 0, NULL, gfx);
		graphe_kill(srv, gfx, fds, false);
		return ;
	}
	new->fork ? NULL : client_init_data(new, srv);
	new->socket = gfx->socket;
	new->sin = gfx->sin;
	new->len = gfx->len;
	new->team = t;
	new->health = new->fork ? new->health : 1260.0f / (float)srv->time;
	command_player_log(srv, 1, new, NULL);
	fds->max = new->socket > fds->max - 1 ? new->socket + 1 : fds->max;
	graphe_kill(srv, gfx, fds, true);
	command_player_send_welcome(srv, new);
}
