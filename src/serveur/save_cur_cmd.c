/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_current_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/28 17:06:12 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/05 19:02:00 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_save_cur_cmd[] =
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

bool		save_cur_cmd(t_client *clt, char *cmd, int action)
{
	oldtonew_cmd(clt);
	if (!clt || !cmd)
		return (false);
	if (!(action != 2 && action != 3 && action != 9))
	{
		clt->fornext_cmd = ft_strdelvoid(ft_strbdup(cmd,
							ft_strlen(g_save_cur_cmd[action])));
	}
	return (true);
}

void		oldtonew_cmd(t_client *clt)
{
	if (clt && !clt->current_cmd && clt->fornext_cmd)
	{
		clt->current_cmd = ft_strdup(clt->fornext_cmd);
		ft_memdel((void **)&clt->fornext_cmd);
	}
	return ;
}

void		erasecur_cmd(t_client *clt)
{
	if (clt && clt->current_cmd)
		ft_memdel((void **)&clt->current_cmd);
	return ;
}
