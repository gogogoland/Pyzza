/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player_inv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 21:31:20 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_piv =
{
	"pin #%i %i %i %i %i %i %i %i %i %i\n"
};

static char	*command_player_inv_get_clt_inv(t_client *clt, int *inv)
{
	char	*ret;

	ret = NULL;
	asprintf(&ret, g_cmd_piv, clt->socket, clt->pos.x, clt->pos.y,
			inv[0], inv[1], inv[2], inv[3], inv[4], inv[5], inv[6]);
	return (ret);
}

void		command_player_inv(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			i;
	t_client	*clt;
	int			player;
	char		*inv;

	i = 0;
	clt = srv->clt;
	while (cmd[i] != '\0' && cmd[i] != '#')
		i++;
	if ((player = command_get_int(i, cmd)) < 1)
		return ;
	while (clt)
	{
		if (clt->socket == player)
			break ;
		clt = clt->next;
	}
	if (!clt)
		return ;
	inv = command_player_inv_get_clt_inv(clt, clt->rsc);
	if (inv)
		send(gfx->socket, inv, strlen(inv), 0);
	ft_memdel((void **)&inv);
}
