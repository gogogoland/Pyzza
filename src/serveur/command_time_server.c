/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_time_server.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/03 18:45:53 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/01 01:18:17 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_time_server = "sgt %d\n";

void		command_time_server(t_fds *fds,
								t_server *srv,
								t_gfx *gfx,
								char *cmd)
{
	char		*box;

	if (!gfx->isgfx)
		command_player(fds, srv, gfx, cmd);
	box = NULL;
	asprintf(&box, g_cmd_time_server, srv->time);
	send(gfx->socket, box, strlen(box), 0);
	ft_memdel((void **)&box);
}
