/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incant_help_acolyte.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/21 15:25:11 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/21 18:22:09 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	incant_lvlup_acolyte(t_server *srv, t_client *clt, int lim_acolyte);
static void	incant_reset_acolyte(t_server *srv, t_client *clt, int lim_acolyte);
static bool	incant_check_acolyte(t_server *srv, t_client *clt, int lim_acolyte);
static bool	incant_init_acolyte(t_server *srv, t_client *clt, int lim_acolyte);
static void	incant_msg_acolyte(t_server *srv, t_client *clt, int lim_acolyte);

static const char	*g_msg_init_aco = "pie %i %i %i #%i";

static void	incant_msg_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	char		*msg;

	asprintf(&msg, g_msg_init_aco, clt->pos.x, clt->pos.y, clt->lvl, clt->name);
	while (lim_acolyte--)
		asprintf(&msg, " #%i", clt->acolyte[lim_acolyte]);
	asprintf(&msg, "\n");
	send_graphe_action(srv, msg, 0, NULL);
	ft_memdel((void **)&msg);
}

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
					&& !player->casting	&& player->pos.x == clt->pos.x
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

static void	incant_reset_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
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

static void	incant_lvlup_acolyte(t_server *srv, t_client *clt, int lim_acolyte)
{
	t_client	*player;

	clt->lvl = (clt->tolvl > 8) ? 8 : clt->tolvl;
	while (lim_acolyte--)
	{
		player = srv->clt;
		while (player->name != clt->acolyte[lim_acolyte])
			player = player->next;
		player->lvl = player->lvl + 1 == clt->tolvl ? clt->tolvl : player->lvl;
	}
}

bool	incant_help_acolyte(t_server *srv, t_client* clt, int lim_acolyte)
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
