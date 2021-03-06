/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 16:04:01 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/05 18:58:25 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_server	*init_map(t_server *srv)
{
	int		z;
	int		y;
	int		x;
	bool	ok;

	ok = true;
	srv->map = NULL;
	if ((!srv || !srv->plateau.y || !srv->plateau.x)
			|| (!(srv->map = (int ***)malloc(srv->plateau.y * sizeof(int **)))))
		ok = !ok;
	y = -1;
	while (ok && ++y < srv->plateau.y && (x = -1) < 0)
	{
		if (!(srv->map[y] = (int **)malloc(srv->plateau.x * sizeof(int *))))
			ok = !ok;
		while (ok && ++x < srv->plateau.x && (z = -1) < 0)
		{
			if (!(srv->map[y][x] = (int *)malloc(8 * sizeof(int))))
				ok = !ok;
			while (ok && ++z < 8)
				srv->map[y][x][z] = 0;
		}
	}
	!ok ? kill_map(srv) : generate_map(srv, NBR_RESRC_CASE_MAX);
	return (srv);
}

void		kill_map(t_server *srv)
{
	int		z;
	int		y;
	int		x;

	y = -1;
	while (srv->map && ++y < srv->plateau.y)
	{
		x = -1;
		while (srv->map && srv->map[y] && ++x < srv->plateau.x)
		{
			z = -1;
			while (srv->map && srv->map[y] && srv->map[y][x] && ++z < 8)
				srv->map[y][x][z] = 0;
			free(srv->map[y][x]);
		}
		free(srv->map[x]);
	}
	if (srv->map)
		free(srv->map);
	srv->map = NULL;
}
