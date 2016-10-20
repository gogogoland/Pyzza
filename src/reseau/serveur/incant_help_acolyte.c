/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incant_help_acolyte.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/21 15:25:11 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/20 12:14:42 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static bool	incant_init_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	int			fix_lim_acolyte;
	t_client	*player;

	fix_lim_acolyte = lim_acolyte;
	if (lim_acolyte)
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
	!(lim_acolyte) ? incant_msg_acolyte(srv, clt, fix_lim_acolyte) :
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
