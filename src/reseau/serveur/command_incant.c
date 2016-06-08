/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_incant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:40:06 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/08 19:52:26 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const int	g_cmd_incant[7][7] =
{
	{1, 1, 0, 0, 0, 0, 0},
	{2, 1, 1, 1, 0, 0, 0},
	{2, 2, 0, 1, 0, 2, 0},
	{4, 1, 1, 2, 0, 1, 0},
	{4, 1, 2, 1, 3, 0, 0},
	{6, 1, 2, 3, 0, 1, 0},
	{6, 2, 2, 2, 2, 2, 1}
};

static const char	*g_cmd_incant_msg[] =
{
	"elevation en cours niveau actuel : %i\n",
	"incantation interrompu niveau actuel : %i\n"
};

static bool	incant_interrupt(t_client *clt, t_server *srv)
{
	int			i;
	bool		stop;
	t_client	*player;
	int			n_player;

	i = 0;
	player = srv->clt;
	stop = (clt->lvl < 8) ? false : true;
	while (player)
	{
		if (player->name > 0 && player->lvl == clt->lvl
				&& player->pos.x == clt->pos.x && player->pos.y == clt->pos.y)
		   n_player++;
		player = player->next;
	}
	stop = (n_player >= g_cmd_incant[clt->lvl - 1][i]) ? stop : true;
	while (!stop && ++i < 7)
		stop = clt->rsc[i - 1] >= g_cmd_incant[clt->lvl - 1][i] ? false : true;
	stop = (clt->tolvl == clt->lvl + 1) ? stop : true;
	if (stop)
		send_graphe_action(srv, clt, 0);
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
	lvlup = (incant_interrupt(clt, srv)) ? 0 : 1;
	while (lvlup && ++i < 7)
		clt->rsc[i - 1] -= g_cmd_incant[clt->lvl - 1][i];
	clt->lvl = (lvlup) ? clt->tolvl : clt->lvl;
	clt->lvl = (clt->lvl > 8) ? 8 : clt->lvl;
	if (asprintf(&msg, g_cmd_incant_msg[lvlup], clt->lvl) > 0)
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
