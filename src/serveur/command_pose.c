/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pose.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 15:18:57 by tbalea            #+#    #+#             */
/*   Updated: 2017/01/19 01:25:16 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_pose[] =
{
	"nourriture\n",
	"linemate\n",
	"deraumere\n",
	"sibur\n",
	"mendiane\n",
	"phiras\n",
	"thystame\n"
};

void	command_pose(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int		r;
	bool	ook;

	r = 0;
	ook = false;
	oldtonew_cmd(clt);
	while (r < 7 && clt->current_cmd && strncmp(g_cmd_pose[r], clt->current_cmd,
							ft_strlen(clt->current_cmd)))
//							ft_strlen(g_cmd_pose[r])))
		r++;
	if (r < 7 && clt->pos.rsc[r] > 0)
		ook = true;
	send_client_action(srv, clt, ook);
	if (!ook)
		return ;
	clt->pos.rsc[r]--;
	srv->map[clt->pos.y][clt->pos.x][r]++;
	send_graphe_action(srv, command_write_msg(clt, 11, r, NULL), 0, NULL);
	send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 1, clt);
	erasecur_cmd(clt);
}
