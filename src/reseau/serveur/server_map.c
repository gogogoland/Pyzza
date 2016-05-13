/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 16:04:01 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/11 15:09:07 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	init_map(t_server *srv)
{
	int		z;
	int		y;
	int		x;
	bool	ok;

ft_putendl("server_map.c	init_map	0");
	ok = true;
	srv->map = NULL;
	if ((!srv || !srv->plateau.y || !srv->plateau.x)
			|| (!(srv->map = (int ***)malloc(srv->plateau.y * sizeof(int **)))))
		ok = !ok ;
ft_putendl("server_map.c	init_map	1");
	y = -1;
	while (ok && ++y < srv->plateau.y && (x = -1) < 0)
	{
ft_putendl("server_map.c	init_map	2");
		if (!(srv->map[y] = (int **)malloc(srv->plateau.x * sizeof(int *))))
			ok = !ok;
ft_putendl("server_map.c	init_map	3");
		while (ok && ++x < srv->plateau.x && (z = -1) < 0)
		{
ft_putendl("server_map.c	init_map	4");
			if (!(srv->map[y][x] = (int *)malloc(8 * sizeof(int))))
				ok = !ok;
ft_putendl("server_map.c	init_map	5");
			while (ok && ++z < 8)
				srv->map[y][x][z] = 0;
ft_putendl("server_map.c	init_map	6");
		}
	}
ft_putendl("server_map.c	init_map	7");
	if (!ok)
		kill_map(srv);
ft_putendl("server_map.c	init_map	8");
}

void	kill_map(t_server *srv)
{
	int		z;
	int		y;
	int		x;

ft_putendl("server_map.c	kill_map	0");
	y = -1;
	while (srv->map && ++y < srv->plateau.y)
	{
ft_putendl("server_map.c	kill_map	1");
		x = -1;
		while (srv->map && srv->map[y] && ++x < srv->plateau.x)
		{
ft_putendl("server_map.c	kill_map	2");
			z = -1;
			while (srv->map && srv->map[y] && srv->map[y][x] && ++z < 8)
				srv->map[y][x][z] = 0;
ft_putendl("server_map.c	kill_map	3");
			free(srv->map[x][y]);
		}
		free(srv->map[x]);
	}
ft_putendl("server_map.c	kill_map	4");
	if (srv->map)
		free(srv->map);
ft_putendl("server_map.c	kill_map	5");
	srv->map = NULL;
}
