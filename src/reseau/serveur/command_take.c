/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_take.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 16:00:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:47:05 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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
	char	**tab;

	i = 0;
	tab = ft_strsplit(cmd, ' ');
	while (tab && tab[++i])
	{
		rsc = 0;
		while (rsc < 7 && strcmp(cmd_pose[rsc], tab[i]))
			rsc++;
		if (rsc == 7 || srv->map[clt->pos.x][clt->pos.y][rsc] == 0)
			continue ;
		clt->pos.rsc[rsc] = srv->map[clt->pos.x][clt->pos.y][rsc];
		srv->map[clt->pos.x][clt->pos.y][rsc] = 0;
	}
	send_graphe_action(srv, clt, 0);
	ft_tabdel(tab);
}
