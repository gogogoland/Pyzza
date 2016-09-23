/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pose.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 15:18:57 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/20 21:41:19 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char *g_msg_cmd_ = "ppo #%i\n";

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
	int		i;
	int		rsc;
	bool	ook;
	char	**tab;

	i = 0;
	ook = false;
	tab = ft_strsplit(cmd, ' ');
	while (tab && tab[++i])
	{
		rsc = 0;
		while (rsc < 7 && strcmp(g_cmd_pose[rsc], tab[i]))
			rsc++;
		if (rsc == 7 || clt->pos.rsc[rsc] == 0)
			continue ;
		ook = true;
		srv->map[clt->pos.x][clt->pos.y][rsc] = clt->pos.rsc[rsc];
		clt->pos.rsc[rsc] = 0;
		send_graphe_action(srv, command_write_msg(clt, 11, 0, NULL), 0, NULL);
		send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 1, clt);
	}
	send_client_action(clt, ook);
	ft_tabdel(tab);
}
