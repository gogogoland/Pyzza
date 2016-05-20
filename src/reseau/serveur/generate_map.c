/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 17:09:02 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/10 22:20:23 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <stdlib.h>

static int		ft_rand_range(int min, int max)
{
	return ((rand() % (max - min + 1)) + min);
}

static int		ft_rand_resrc(void)
{
	int	rand_resrc;

	rand_resrc = ft_rand_range(0, 100);
	if (rand_resrc < 10)
		return (6);
	else if (rand_resrc < 30)
		return (ft_rand_range(4, 5));
	else if (rand_resrc < 50)
		return (ft_rand_range(1, 3));
	return (-1);
}

static void		ft_assign_collect(t_server *srv, int y, int x, int nbr)
{
	int			r_collect;

	r_collect = ft_rand_range(0, 100);
	if (r_collect < 15)
		srv->map[y][x][0] += nbr;
	else if (r_collect < 65)
	{
		r_collect = ft_rand_resrc();
		if (r_collect != -1)
			srv->map[y][x][r_collect] += nbr;
	}
}

static void		ft_assign_tile(t_server *srv, int y, int x)
{
	int		r_tile;

	r_tile = ft_rand_range(0, 100);
	if (r_tile < 25)
		srv->map[y][x][7] = 2;
	else if (r_tile < 50)
		srv->map[y][x][7] = 1;
	else if (r_tile < 100)
		srv->map[y][x][7] = 0;
}

void			generate_map(t_server *srv, int nbr_resrc_case)
{
	int		x;
	int		y;
	int		r;
	int		nbr;

	y = -1;
	srand(time(NULL));
	while (++y < srv->plateau.y)
	{
		x = -1;
		while (++x < srv->plateau.x)
		{
			ft_assign_tile(srv, y, x);
			nbr = -1;
			r = ft_rand_range(1, nbr_resrc_case);
			while (++nbr < r)
				ft_assign_collect(srv, y, x, nbr);
		}
	}
}
