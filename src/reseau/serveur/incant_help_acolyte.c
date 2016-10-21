/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incant_help_acolyte.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/21 15:25:11 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/21 23:07:07 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static bool	incant_init_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	int			fix_lim_acolyte;
	t_client	*player;

	fix_lim_acolyte = lim_acolyte;
	if ((clt->tolvl = clt->lvl + 1) < 9 && lim_acolyte)
	{
		player = srv->clt;
		clt->acolyte = (int *)malloc(lim_acolyte * sizeof(int));
		while (player && lim_acolyte)
		{
			if (player->lvl == clt->lvl && player->pos.y == clt->pos.y
					&& !player->casting && player->pos.x == clt->pos.x
					&& player->name > 0 && player->name != clt->name)
			{
				player->casting = true;
				lim_acolyte--;
				clt->acolyte[fix_lim_acolyte - lim_acolyte] = player->name;
			}
			player = player->next;
		}
	}
	clt->casting = true;
	!(lim_acolyte) ? incant_msg_acolyte(srv, clt, fix_lim_acolyte, 0) :
		incant_reset_acolyte(srv, clt, fix_lim_acolyte);
	return (lim_acolyte ? false : true);
}

static bool	incant_check_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	t_client	*player;
	int			i;

	i = -1;
	while (++i < lim_acolyte)
	{
		player = srv->clt;
		while (player && !(clt->acolyte[i] == player->name
				&& player->lvl == clt->lvl && player->casting
				&& player->pos.x == clt->pos.x && player->pos.y == clt->pos.y))
			player = player->next;
		if (!player)
			return (false);
	}
	return (true);
}

bool		incant_help_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	if (!clt->acolyte && !clt->casting)
		return (incant_init_acolyte(srv, clt, lim_acolyte));
	else if (clt->acolyte)
	{
		if (!incant_check_acolyte(srv, clt, lim_acolyte))
		{
			incant_reset_acolyte(srv, clt, lim_acolyte);
			return (false);
		}
	}
	else if (clt->casting && lim_acolyte)
		return (false);
	return (true);
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
	int			lim_save;
	t_client	*player;

	oldlvl = srv->lvl;
	clt->lvl = (clt->tolvl > 8) ? 8 : clt->tolvl;
	srv->lvl = clt->lvl > srv->lvl ? clt->lvl : srv->lvl;
	lim_save = lim_acolyte;
	while (lim_acolyte--)
	{
		player = srv->clt;
		while (player->name != clt->acolyte[lim_acolyte])
			player = player->next;
		player->lvl = player->lvl + 1 == clt->tolvl ? clt->tolvl : player->lvl;
	}
	incant_msg_acolyte(srv, clt, lim_save, 1);
	send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 0, NULL);
	incant_reset_acolyte(srv, clt, lim_acolyte);
	if (srv->lvl == 8 && srv->lvl != oldlvl)
	{
		send_graphe_action(srv, command_write_msg(clt, 13, clt->team, NULL),
							0, NULL);
	}
}
