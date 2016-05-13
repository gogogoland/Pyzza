/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_maps.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 17:04:21 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/07 17:11:29 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	init_map(t_server *srv)
{
	int         i;
	int         j;
	int         k;

	i = -1;
	srv->map = (int ***)malloc(srv->plateau.y * sizeof(int **));
	while (srv && srv->map && ++i < srv->plateau.y && srv->map[i] && (j = -1))
	{
		srv->map[i]= (int **)malloc(srv->plateau.x * sizeof(int *));
		while (srv->map[i] && ++j < srv->plateau.x && (k = -1))
		{
			srv->map[i][j]= (int *)malloc(7 * sizeof(int ));
			while (++k < 6)
				srv->map[i][j][k] = 0;
		}
	}
}

void	reset_map(int ***map, int y, int x)
{
	int	xt;
	int	r;

	while (y-- && (xt = x))
	{
		while (xt-- && (r = 6))
		{
			while (r--)
				map[y][x][r] = 0;
		}
	}
}

void	destroy_map(int ***map, int y, int x)
{
	int	xt;
	int	r;

	while (y-- && (xt = x))
	{
		while (xt-- && (r = 6))
		{
			while (r--)
			{
				map[y][x][r] = 0;
				free(map[y][x][r]);
			}
			free(map[y][x]);
		}
		free(map[y]);
	}
	free(map);
}
