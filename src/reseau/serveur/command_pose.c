/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pose.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 15:18:57 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/20 17:03:45 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_pose[] =
{
	"nourriture",
	"linemate",
	"deraumere",
	"sibur",
	"mendiane",
	"phiras",
	"thystame"
};

void	command_pose(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int		r;
	bool	ook;

	ook = false;
	r = 0;
	while (r < 7 && strncmp(g_cmd_pose[r], clt->current_cmd,
							ft_strlen(g_cmd_pose[r])))
		r++;
	if (r < 7 && clt->pos.rsc[r] > 0)
		ook = true;
	send_client_action(clt, ook);
	if (!ook)
		return ;
	clt->pos.rsc[r]--;
	srv->map[clt->pos.y][clt->pos.x][r]++;
	send_graphe_action(srv, command_write_msg(clt, 11, r, NULL), 0, NULL);
	send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 1, clt);
}
