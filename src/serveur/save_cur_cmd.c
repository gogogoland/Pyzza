/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_current_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/28 17:06:12 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/28 18:40:05 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_save_cur_cmd[] =
{
	"avance\n",
	"droite\n",
	"gauche\n",
	"voir\n",
	"inventaire\n",
	"prend",
	"pose",
	"expulse\n",
	"broadcast",
	"incantation\n",
	"fork\n",
	"connect_nbr\n",
	"eat\n"
};

bool		save_cur_cmd(t_client *clt, char *cmd, int action)
{
	if (!clt || !cmd)
		return (false);
	if (clt->current_cmd)
		ft_memdel((void **)&clt->current_cmd);
	clt->current_cmd = ft_strdelvoid(ft_strbdup(cmd,
						ft_strlen(g_save_cur_cmd[action])));
	return (true);
}
