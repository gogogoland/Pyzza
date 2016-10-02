/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incant_help_acolyte.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/21 15:25:11 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/02 20:14:46 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_msg_init_aco = "pic %i %i %i #%i";

void	incant_msg_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	int			error;
	char		*msg;
	char		*tmp;

	msg = NULL;
	if (asprintf(&msg, g_msg_init_aco,
				clt->pos.x, clt->pos.y, clt->lvl, clt->name) < 0)
		return ;
	while (lim_acolyte--)
	{
		error = asprintf(&tmp, "%s #%i", msg, clt->acolyte[lim_acolyte]);
		ft_memdel((void **)&msg);
		if (error < 0)
			return ;
		error = asprintf(&msg, "%s", tmp);
		ft_memdel((void **)&tmp);
		if (error < 0)
			return ;
	}
	error = asprintf(&tmp, "%s\n", msg);
	ft_memdel((void **)&msg);
	send_graphe_action(srv, tmp, 0, NULL);
}

void	incant_reset_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	t_client	*player;
	int			i;

	i = -1;
	while (++i < lim_acolyte)
	{
		player = srv->clt;
		while (player && player->name != clt->acolyte[i])
			player = player->next;
		if (player->lvl == clt->lvl && player->pos.y == clt->pos.y
				&& player->pos.x == clt->pos.x)
			player->casting = false;
		clt->acolyte[i] = 0;
	}
	if (clt->acolyte)
		free(clt->acolyte);
	clt->acolyte = NULL;
	clt->casting = false;
}

void	incant_lvlup_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	int			oldlvl;
	t_client	*player;

	oldlvl = srv->lvl;
	clt->lvl = (clt->tolvl > 8) ? 8 : clt->tolvl;
	srv->lvl = clt->lvl > srv->lvl ? clt->lvl : srv->lvl;
	while (lim_acolyte--)
	{
		player = srv->clt;
		while (player->name != clt->acolyte[lim_acolyte])
			player = player->next;
		send_graphe_action(srv, command_write_msg(player, 12, player->lvl,
												NULL), 0, NULL);
		player->lvl = player->lvl + 1 == clt->tolvl ? clt->tolvl : player->lvl;
	}
	send_graphe_action(srv, command_write_msg(clt, 12, clt->lvl, NULL), 0,
						NULL);
	send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 0, NULL);
	incant_reset_acolyte(srv, clt, lim_acolyte);
	if (srv->lvl == 8 && srv->lvl != oldlvl)
	{
		send_graphe_action(srv, command_write_msg(clt, 13, clt->team, NULL),
							0, NULL);
	}
}
