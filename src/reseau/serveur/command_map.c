/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 23:14:27 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		command_map(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			x;
	int			y;
//	int			child;
	char		*box;
	t_client	*clt;

/*	if ((child = fork()) < 0)
		return ;
	else if (child != 0)
		return ;*/
	x = -1;
	while (++x < srv->plateau.x)
	{
		y = -1;
		while (++y < srv->plateau.y)
			command_box_content(gfx, x, y, srv->map[x][y]);
	}
//	exit(0);
}
