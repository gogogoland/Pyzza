/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_incant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:40:06 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/22 14:40:36 by tbalea           ###   ########.fr       */
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
	"pie %i %i %s\n"
};

static bool	incant_interrupt(t_client *clt, t_server *srv)
{
	int			i;
	int			stop;
	t_client	*player;
	int			n_player;

	i = 0;
	player = srv->clt;
	stop = (clt->lvl < 8) ? 0 : 1;
	stop = incant_help_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0]) ? stop :
			1;
	while (!stop && ++i < 7)
		stop = clt->pos.rsc[i - 1] >= g_cmd_incant[clt->lvl - 1][i] ? 0 : 1;
	stop = (clt->tolvl == clt->lvl + 1) ? stop : 1;
	return (stop ? true : false);
}

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
		return ;
	lvlup = (incant_interrupt(clt, srv)) ? 0 : 1;
	while (lvlup && ++i < 7)
		clt->pos.rsc[i - 1] -= g_cmd_incant[clt->lvl - 1][i];
	clt->tolvl = lvlup ? clt->tolvl : clt->lvl;
	incant_lvlup_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0]);
	asprintf(&msg, g_msg_cmd_incant[lvlup], clt->lvl);
	send(clt->socket, msg, strlen(msg), 0);
	ft_memdel((void **)&msg);
	asprintf(&msg, g_msg_cmd_incant[2], clt->pos.x, clt->pos.y,
				lvlup ? "ok" : "ko");
	send(clt->socket, msg, strlen(msg), 0);
	ft_memdel((void **)&msg);
}

bool		incant_process(t_client *clt, t_server *srv)
{
	if (clt && clt->time == 0.0f)
		return (false);
	else if (clt && clt->action == 10 && !incant_interrupt(clt, srv))
	{
		incant_msg_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0]);
		incant_reset_acolyte(srv, clt, g_cmd_incant[clt->lvl - 1][0]);
		clt->action = 0;
		clt->time = 0.0f;
		return (false);
	}
	return (true);
}
