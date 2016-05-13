/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/10 17:50:31 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static char	*command_map_add_arg(char *str, t_client *clt, int n)
{
	char	*ret;
	char	*arg;

	if (n >= 0)
		arg = ft_itoa(n);
	else
	{
		arg = command_map_add_arg(arg, clt, clt->socket);
		arg = command_map_add_arg(arg, clt, clt->pos.x);
		arg = command_map_add_arg(arg, clt, clt->pos.y);
	}
	ret = ft_strcjoin(str, arg, ' ');
	ft_memdel((void **)&arg);
	ft_memdel((void **)&str);
	return (ret);
}

void		command_map(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	int			x;
	int			y;
	int			z;
	char		*map;
	t_client	*clt;

	x = -1;
	map = NULL;
	clt = srv->clt;
	while (++x < srv->plateau.x)
	{
		y = -1;
		while (++y < srv->plateau.y)
		{
			z = -1;
			while (++z < 8)
				map = command_map_add_arg(map, clt, srv->map[x][y][z]);
		}
	}
	while (clt)
	{
		if (clt->socket > 0)
			map = command_map_add_arg(map, clt, -1);
		clt = clt->next;
	}
	send(gfx->socket, map, strlen(map), 0);
	ft_memdel((void **)&map);
}
