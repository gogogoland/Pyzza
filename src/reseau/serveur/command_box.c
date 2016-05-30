/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_box.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/30 20:32:23 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/30 21:05:26 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		command_box(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			i;
	int			x;
	int			y;

	i = 0;
	x = -1;
	y = -1;
	while (cmd[i] != '\0' && (cmd[i] < '0' || cmd[i] > '9'))
		i++;
	x = command_get_int(i - 1, cmd);
	while (cmd[i] != '\0' && cmd[i] >= '0' && cmd[i] <= '9')
		i++;
	while (cmd[i] != '\0' && (cmd[i] < '0' || cmd[i] > '9'))
		i++;
	while (cmd[i] != '\0' && !isdigit(cmd[i]))
		i++;
	y = command_get_int(i - 1, cmd);
	if (x >= 0 && x < srv->plateau.x && y >=0 && y < srv->plateau.y)
		command_box_content(gfx, x, y, srv->map[x][y]);
}
