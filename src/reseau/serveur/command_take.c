/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_take.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 16:00:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/18 19:11:02 by tbalea           ###   ########.fr       */
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
	}
	send_graphe_action(srv, clt, 0);
	send_client_action(clt, ook);
	ft_tabdel(tab);
}
