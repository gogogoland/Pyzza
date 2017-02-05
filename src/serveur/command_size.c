/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 14:27:13 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/01 01:18:14 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_size = "msz %i %i\n";

void		command_size(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	char		*box;

	if (!gfx->isgfx)
		command_player(fds, srv, gfx, cmd);
	box = NULL;
	asprintf(&box, g_cmd_size, srv->plateau.x, srv->plateau.y);
	send(gfx->socket, box, strlen(box), 0);
	ft_memdel((void **)&box);
}
