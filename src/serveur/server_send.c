/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_send.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 18:38:03 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 19:57:51 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

typedef void	(*t_x_gfx)(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd);

static const t_x_gfx g_tfg[] =
{
	command_graphe,
	command_team_name,
	command_map,
	command_box,
	command_player_inv,
	command_player_lvl,
	command_player_pos,
	command_size,
	command_time_server,
	command_time_change,
	command_player
};

typedef void	(*t_x_clt)(t_fds *fds, t_server *srv, t_client *clt, char *cmd);

static const t_x_clt g_tfc[] =
{
	command_nbr_co,
	command_inventory,
	command_take,
	command_msg,
	command_eat,
	command_forward,
	command_right,
	command_left,
	command_seek,
	command_pose,
	command_eject,
	command_fork,
	command_incant,
	command_death
};

static const char	*g_gfx_cmd[] =
{
	"GRAPHIC",
	"tna",
	"mct",
	"bct ",
	"pin #",
	"plv #",
	"ppo #",
	"msz",
	"sgt",
	"sst "
};

static const char	*g_clt_cmd[] =
{
	"connect_nbr",
	"inventaire",
	"prend",
	"broadcast",
	"mange",
	"avance",
	"droite",
	"gauche",
	"voir",
	"pose",
	"expulse",
	"fork",
	"incantation"
};

static const int	g_cmd_time[] =
{
	0,
	1000000,
	7000000,
	7000000,
	7000000,
	7000000,
	7000000,
	7000000,
	7000000,
	7000000,
	7000000,
	42000000,
	300000000
};

static char	*time_lapse(t_fds *fds, t_server *srv, int tim)
{
	t_client	*clt;

	clt = srv->clt;
	while (clt)
	{
		if (clt->time > 0 && clt->time <= tim && clt->fork && !clt->socket)
		{
			srv->egg--;
			send_graphe_action(srv, command_write_msg(clt, 2, 0, 0), 0, 0);
		}
		if ((clt->time -= tim) < 0)
			clt->time = 0;
		if ((clt->health -= tim) < 0)
			clt->health = 0;
		if (clt->health <= 0 && (clt->fork || clt->socket))
			g_tfc[13](fds, srv, clt, NULL);
		clt = clt->next;
	}
	return (NULL);
}

static int	time_action(int n, t_client *clt, t_server *srv)
{
	int	action;

	if (!clt)
		return (0);
	action = (clt->time == 0 && clt->action > 0) ? clt->action : 0;
	if (clt->time == 0 && !clt->socket)
	{
		srv->egg--;
		send_graphe_action(srv, command_write_msg(clt, 2, 0, NULL), 0, NULL);
	}
	if (clt->time == 0 && (clt->action = n < 13 ? n + 1 : 0) == 12)
		send_graphe_action(srv, command_write_msg(clt, 1, 0, NULL), 0, NULL);
	if (clt->time <= 0 && n < 13)
		clt->time = g_cmd_time[n] / srv->time;
	return (action);
}

void		send_client(t_fds *fds, t_server *srv, int tim)
{
	int			n;
	char		*cmd;
	t_gfx		*gfx;
	t_client	*clt;

	cmd = time_lapse(fds, srv, tim);
	while (tim >= 0 && --fds->max > 0 && !(n = 0))
	{
		if (!(clt = srv->clt) && !FD_ISSET(fds->max, &fds->wr))
			continue ;
		while (clt != NULL && clt->socket != fds->max)
			clt = clt->next;
		gfx = srv->gfx;
		while (gfx != NULL && gfx->socket != fds->max)
			gfx = gfx->next;
		if (!incant_process(clt, srv) || (!clt && gfx))
			cmd = !clt ? ring_send(srv, gfx->ring) : ring_send(srv, clt->ring);
		while (cmd && n < (clt ? 13 : 10) && strncmp(cmd, (!clt ? g_gfx_cmd[n] :
				g_clt_cmd[n]), strlen(!clt ? g_gfx_cmd[n] : g_clt_cmd[n])) != 0)
			n++;
		(!clt && cmd && 0 <= n && n < 11) ? g_tfg[n](fds, srv, gfx, cmd) : NULL;
		if ((n = time_action((save_cur_cmd(clt, cmd, n) ? n : 13), clt, srv)))
			g_tfc[n - 1](fds, srv, clt, cmd);
		ft_memdel((void **)&cmd);
	}
}

void		send_client_action(t_server *srv, t_client *clt, bool ok)
{
	char	*msg;

	msg = NULL;
	if (clt && clt->socket)
	{
		send(clt->socket, ok ? "ok\n" : "ko\n", 3, 0);
		asprintf(&msg, "Result of command from player #%i is %s",
				clt->name, ok ? "ok\n" : "ko\n");
		server_log(srv, msg);
		ft_memdel((void **)&msg);
	}
}

void		send_graphe_action(t_server *srv, char *msg,
								int spec, t_client *clt)
{
	t_gfx	*gfx;

	if (!msg)
		return ;
	gfx = srv->gfx;
	while (msg && gfx)
	{
		if (gfx->isgfx && gfx->socket > 0)
		{
			send(gfx->socket, msg, strlen(msg), 0);
			if (spec == 1 && clt)
			{
				command_box_content(gfx, clt->pos.x, clt->pos.y,
									srv->map[clt->pos.y][clt->pos.x]);
			}
		}
		gfx = gfx->next;
	}
	server_log(srv, msg);
	ft_memdel((void **)&msg);
}
