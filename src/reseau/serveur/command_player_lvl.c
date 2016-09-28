/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player_lvl.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/28 19:48:59 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_plv = "plv #%i %i\n";

void		command_player_lvl(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			i;
	t_client	*clt;
	int			player;
	char		*box;

	if (!gfx->isgfx)
		return ;
	box = NULL;
	clt = srv->clt;
	while (cmd[i] != '\0' && cmd[i] != '#')
		i++;
	if ((player = command_get_int(i, cmd)) < 1)
		return ;
	while (clt)
	{
		if (clt->name == player)
			break ;
		clt = clt->next;
	}
	if (!clt)
		return command_graphical_bad_parameters(fds, srv, gfx, cmd);
	asprintf(&box, g_cmd_plv, clt->name, clt->lvl);
	if (box)
		send(gfx->socket, box, strlen(box), 0);
	ft_memdel((void **)&box);
}
