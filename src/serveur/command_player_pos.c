/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player_pos.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/14 18:27:21 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_pos = "ppo #%i %i %i %i\n";

void		command_player_pos(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			i;
	t_client	*clt;
	int			player;
	char		*pos;

	if (!gfx->isgfx)
		command_player(fds, srv, gfx, cmd);
	i = 0;
	pos = NULL;
	clt = srv->clt;
	while (cmd[i] != '\0' && cmd[i] != '#')
		i++;
	if ((player = command_get_int(i, cmd)) < 1)
		return ;
	while (clt && clt->name != player)
		clt = clt->next;
	if (!clt || clt->socket <= 0)
		return (command_graphical_bad_parameters(fds, srv, gfx, cmd));
	asprintf(&pos, g_cmd_pos, clt->name, clt->pos.x, clt->pos.y, clt->sens);
	if (pos)
		send(gfx->socket, pos, strlen(pos), 0);
	ft_memdel((void **)&pos);
}
