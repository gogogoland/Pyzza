/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 17:09:02 by nd-heyge          #+#    #+#             */
/*   Updated: 2017/02/08 18:49:31 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_log_map_data =
{
	"y:%i x:%i r0:%i r1:%i r2:%i r3:%i r4:%i r5:%i r6:%i ground:%i\n"
};

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
	if (r_collect < 50)
		srv->map[y][x][0] += nbr * 10;
	else if (r_collect < 100)
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
	char	*log;

	y = -1;
	srand(time(NULL));
	while (++y < srv->plateau.y && (x = -1))
	{
		while (++x < srv->plateau.x && (nbr = -1))
		{
			ft_assign_tile(srv, y, x);
			r = nbr_resrc_case;
			while (++nbr < r)
				ft_assign_collect(srv, y, x, nbr);
			if (asprintf(&log, g_log_map_data, y, x, srv->map[y][x][0],
						srv->map[y][x][1], srv->map[y][x][2], srv->map[y][x][3],
						srv->map[y][x][4], srv->map[y][x][5], srv->map[y][x][6],
						srv->map[y][x][7]))
				server_log(srv, log);
			if (log)
				ft_memdel((void **)&log);
		}
	}
}
