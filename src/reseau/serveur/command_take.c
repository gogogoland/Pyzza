/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_take.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 16:00:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/01 21:11:14 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_take[] =
{
	"nourriture",
	"linemate",
	"deraumere",
	"sibur",
	"mendiane",
	"phiras",
	"thystame"
};

void	command_take(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int		i;
	int		r;
	bool	ook;

	ook = false;
	r = 0;
	while (r < 7 && strncmp(g_cmd_take[r], clt->current_cmd,
							ft_strlen(g_cmd_take[r])))
		r++;
	if (r < 7 && srv->map[clt->pos.x][clt->pos.y][r] > 0)
		ook = true;
	send_client_action(clt, ook);
	if (!ook)
		return ;
	clt->pos.rsc[r]++;
	srv->map[clt->pos.x][clt->pos.y][r]--;
	send_graphe_action(srv, command_write_msg(clt, 10, 0, NULL), 0, NULL);
	send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 1, clt);
}
