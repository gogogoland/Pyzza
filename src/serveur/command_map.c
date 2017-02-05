/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/14 17:06:08 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		command_map(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			x;
	int			y;
	int			child;
	char		*box;
	t_client	*clt;

	if (!gfx->isgfx)
		command_player(fds, srv, gfx, cmd);
	if (srv->bonus_fork && (child = fork()) < 0)
		return ;
	else if (srv->bonus_fork && child != 0)
		return ;
	x = -1;
	while (++x < srv->plateau.x)
	{
		y = -1;
		while (++y < srv->plateau.y)
			command_box_content(gfx, x, y, srv->map[y][x]);
	}
	if (srv->bonus_fork)
		exit(0);
}
