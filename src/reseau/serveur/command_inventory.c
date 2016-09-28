/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_inventory.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:32:11 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/28 18:36:20 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_inv_rsc[] =
{
	"nourriture",
	"linemate",
	"deraumere",
	"sibur",
	"mendiane",
	"phirase",
	"thystame"
};

static const int	g_cmd_inv_num = 7;

void	command_inventory(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int		i;
	char	*tmp;
	char	*rsc;

	i = -1;
	tmp = NULL;
	rsc = NULL;
	while (++i < g_cmd_inv_num)
	{
		if (!i)
			asprintf(&tmp, "%s %i", g_cmd_inv_rsc[i], clt->pos.rsc[i]);
		else
			asprintf(&tmp, "%s, %s %i", rsc, g_cmd_inv_rsc[i], clt->pos.rsc[i]);
		if (rsc)
			ft_memdel((void **)&rsc);
		asprintf(&rsc, (i + 1 == g_cmd_inv_num) ? "%s\n" : "%s", tmp);
		if (tmp)
			ft_memdel((void **)&tmp);
	}
	send(clt->socket, rsc, strlen(rsc), 0);
	ft_memdel((void **)&rsc);
}
