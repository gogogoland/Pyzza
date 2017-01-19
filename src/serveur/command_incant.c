/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_incant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:40:06 by tbalea            #+#    #+#             */
/*   Updated: 2017/01/18 22:51:23 by tbalea           ###   ########.fr       */
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

static bool	incant_interrupt(t_client *clt, t_server *srv)
{
	int			i;
	int			stop;
	t_client	*player;
	int			n_player;

	i = 0;
	player = srv->clt;
	stop = (clt->health > 0.0f) ? 0 : 1;
	stop = (clt->lvl < 8) ? stop : 1;
	stop = incant_help_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0]) ? stop :
			1;
	while (!stop && ++i < 7)
		stop = clt->pos.rsc[i] >= g_cmd_incant[clt->lvl - 1][i] ? 0 : 1;
	stop = (clt->tolvl == clt->lvl + 1) ? stop : 1;
	return (stop ? true : false);
}

void		command_incant(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int			i;
	int			lvlup;

	i = 0;
	lvlup = 1;
	lvlup = (incant_interrupt(clt, srv)) ? 0 : 1;
	while (lvlup && ++i < 7)
		clt->pos.rsc[i] -= g_cmd_incant[clt->lvl - 1][i];
	clt->tolvl = lvlup ? clt->tolvl : clt->lvl;
	incant_lvlup_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0]);
}

bool		incant_process(t_client *clt, t_server *srv)
{
	if (clt && clt->time == 0.0f)
		return (false);
	else if (clt && clt->action == 13 && incant_interrupt(clt, srv))
	{
		incant_msg_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0], -1);
		incant_reset_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0]);
		clt->action = 0;
		clt->time = 0.0f;
		return (false);
	}
	return (true);
}
