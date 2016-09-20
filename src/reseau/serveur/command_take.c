/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_take.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 16:00:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/20 21:42:25 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char *g_msg_cmd_ = "ppo #%i\n";

static const char	*cmd_pose[] =
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
	int		rsc;
	bool	ook;
	char	**tab;

	i = 0;
	ook = false;
	tab = ft_strsplit(cmd, ' ');
	while (tab && tab[++i])
	{
		rsc = 0;
		while (rsc < 7 && strcmp(cmd_pose[rsc], tab[i]))
			rsc++;
		if (rsc == 7 || srv->map[clt->pos.x][clt->pos.y][rsc] == 0)
			continue ;
		ook = true;
		clt->pos.rsc[rsc] = srv->map[clt->pos.x][clt->pos.y][rsc];
		srv->map[clt->pos.x][clt->pos.y][rsc] = 0;
		send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 0, NULL);
		send_graphe_action(srv, command_write_msg(clt, 8, 0, NULL), 1, clt);
	}
	send_client_action(clt, ook);
	ft_tabdel(tab);
}
