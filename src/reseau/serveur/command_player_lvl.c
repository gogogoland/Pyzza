/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_player_lvl.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/14 18:27:44 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_plv = "plv #%i %i\n";

void		command_player_lvl(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			i;
	t_client	*clt;
	int			player;
	char		*lvl;

	if (!gfx->isgfx)
		command_player(fds, srv, gfx, cmd);
	i = 0;
	lvl = NULL;
	clt = srv->clt;
	while (cmd[i] != '\0' && cmd[i] != '#')
		i++;
	if ((player = command_get_int(i, cmd)) < 1)
		return ;
	while (clt && clt->name != player)
		clt = clt->next;
	if (!clt || clt->socket <= 0)
		return (command_graphical_bad_parameters(fds, srv, gfx, cmd));
	asprintf(&lvl, g_cmd_plv, clt->name, clt->lvl);
	if (lvl)
		send(gfx->socket, lvl, strlen(lvl), 0);
	ft_memdel((void **)&lvl);
}
