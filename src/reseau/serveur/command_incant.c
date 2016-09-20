/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_incant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:40:06 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/20 22:16:10 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const int	g_cmd_incant[7][7] =
{
	{0, 1, 0, 0, 0, 0, 0},
	{1, 1, 1, 1, 0, 0, 0},
	{1, 2, 0, 1, 0, 2, 0},
	{3, 1, 1, 2, 0, 1, 0},
	{3, 1, 2, 1, 3, 0, 0},
	{5, 1, 2, 3, 0, 1, 0},
	{5, 2, 2, 2, 2, 2, 1}
};

static const char	*g_msg_cmd_incant[] =
{
	"elevation en cours niveau actuel : %i\n",
	"incantation interrompu niveau actuel : %i\n",
	"pie %i %i %i #%i"
};

static bool	incant_helping_player(t_client *clt, t_server *srv)
{
	int		n_player;
	char	*msg;
	int		i;

	n_player ==  g_cmd_incant[clt->lvl - 1][0];
	player = srv->clt;
	if (clt->casting)
		return (false)
	if (!clt->acolytes)
	{
		while (player && n_player)
		{
			if (player->name > 0 && player->lvl == clt->lvl
					&& !player->casting	&& player->pos.x == clt->pos.x
					&& player->pos.y == clt->pos.y && player->name != clt->name
					&& n_player > 0)
				n_player--;
			player = player->next;
		}
		if (n_player == 0)
		{
			acolyte = (int*)malloc(g_cmd_incqnt[clt->lvl - 1][0] * sizeof(int));
			asprintf(&msg, g_msg_cmd_incant[2],
					clt->pos.x, clt->pos.y, clt->lvl, clt->name);
			while (player && n_player < g_cmd_incant[clt->lvl - 1][0])
			{
				if (player->lvl == clt->lvl && player->pos.y == clt->pos.y
						&& !player->casting	&& player->pos.x == clt->pos.x
						&& player->name > 0 && player->name != clt->name
						&& n_player < g_cmd_incant[clt->lvl - 1][0])
				{
					asprintf(&msg, " #%i", player->name);
					player->casting = true;
					n_player--;
					clt->acolyte[n_player] = player->name;
				}
				player->prev;
			}
			asprintf(&msg, "\n");
			send_graphe_action(srv, msg, 0, NULL);
			return (true);
		}
		else
			return (false);
	}
	else
	{
		i = -1;
		while ()
			//	TODO CHECK ACOLYTE
	}
}

static bool	incant_interrupt(t_client *clt, t_server *srv)
{
	int			i;
	bool		stop;
	t_client	*player;
	int			n_player;

	i = 0;
	player = srv->clt;
	stop = (clt->lvl < 8) ? false : true;
	while (!stop && player)
	{
		if (player->name > 0 && player->lvl == clt->lvl
				&& player->pos.x == clt->pos.x && player->pos.y == clt->pos.y)
			n_player++;
		player = player->next;
	}
	stop = (n_player >= g_cmd_incant[clt->lvl - 1][i]) ? stop : true;
	while (!stop && ++i < 7)
	{
		stop = clt->pos.rsc[i - 1] >= g_cmd_incant[clt->lvl - 1][i]
				? false : true;
	}
	stop = (clt->tolvl == clt->lvl + 1) ? stop : true;
	if (stop)
		send_graphe_action(srv, command_write_msg(clt, 5, 0, NULL), 0, NULL);
	return (stop);
}

//	TODO:
//	*	lvlup other player
//	*	special msg for graphical client
void		command_incant(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int			i;
	char		*msg;
	int			lvlup;

	i = 0;
	msg = NULL;
	lvlup = 1;
	while (lvlup && ++i < 7)
		lvlup = (clt->pos.rsc[i - 1] >= g_cmd_incant[clt->lvl - 1][i]) ? 1 : 0;
	if (!lvlup)
		return;
	lvlup = (incant_interrupt(clt, srv)) ? 0 : 1;
	while (lvlup && ++i < 7)
		clt->pos.rsc[i - 1] -= g_cmd_incant[clt->lvl - 1][i];
	clt->lvl = (lvlup) ? clt->tolvl : clt->lvl;
	clt->lvl = (clt->lvl > 8) ? 8 : clt->lvl;
	if (asprintf(&msg, g_msg_cmd_incant[lvlup], clt->lvl) > 0)
	{
		send(clt->socket, msg, strlen(msg), 0);
		ft_memdel((void **)&msg);
	}
}

bool		incant_process(t_client *clt, t_server *srv)
{
	if (clt && clt->time == 0.0f)
		return (false);
	else if (clt && clt->action == 9 && !incant_interrupt(clt, srv))
	{
		clt->action = 0;
		clt->time = 0.0f;
		return (false);
	}
	return (true);
}
