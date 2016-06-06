/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player_lvl.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 21:31:31 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_plv =
{
	"plv #%i %i\n"
};

void		command_player_lvl(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			i;
	t_client	*clt;
	int			player;
	char		*box;

	box = NULL;
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
	asprintf(&box, g_cmd_plv, clt->socket, clt->lvl);
	if (box)
		send(gfx->socket, box, strlen(box), 0);
	ft_memdel((void **)&box);
}