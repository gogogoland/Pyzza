/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_int.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/02 17:21:57 by croy              #+#    #+#             */
/*   Updated: 2016/10/02 20:08:24 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_parser_int_flag[] =
{
	"-p",
	"-x",
	"-y",
	"-c",
	"-t",
};

static const char	*g_parser_int_name[] =
{
	"port",
	"width",
	"height",
	"Number of player",
	"time",
	"team"
};

static const int	g_parser_int_limit[] =
{
	P_MIN,
	P_MAX,
	X_MIN,
	X_MAX,
	Y_MIN,
	Y_MAX,
	C_MIN,
	C_MAX,
	TIME_MIN,
	TIME_MAX
};

static const char	*g_parser_int_log[] =
{
	"[ERROR] : no correct value after '%s'.\n",
	"[ERROR] : '%s' (%s) value must be between %i and %i.\n"
};

static bool	parser_int_log(t_server *srv, int type, int flag)
{
	char	*log;

	if (asprintf(&log, g_parser_int_log[type], g_parser_int_flag[flag],
				g_parser_int_name[flag], g_parser_int_limit[flag * 2],
				g_parser_int_limit[flag * 2 + 1]))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
	return (false);
}

static bool	parser_int_check_n_get(t_server *srv, int *val)
{
	bool	ook;
	int		i;

	i = -1;
	ook = true;
	while (++i < 5)
	{
		if (val[i] <= g_parser_int_limit[i * 2]
				|| val[i] >= g_parser_int_limit[(i * 2) + 1])
			ook = parser_int_log(srv, 1, i);
	}
	srv->time = val[4];
	srv->player_max = val[3];
	srv->plateau.y = val[2];
	srv->plateau.x = val[1];
	srv->port = val[0];
	return (ook);
}

bool		parser_int(int ac, char **av, t_server *srv)
{
	int		i;
	int		j;
	int		val[5];
	bool	ook;

	i = 5;
	ook = true;
	while (--i >= 0)
		val[i] = 0;
	while ((j = -1) && ++i < ac)
	{
		while (++j < 5)
		{
			if (!strcmp(av[i], g_parser_int_flag[j]))
			{
				if (i < (ac - 1) && ft_strisdigit(av[i + 1]))
					val[j] = atoi(av[i + 1]);
				else
					ook = parser_int_log(srv, 0, j);
			}
		}
	}
	if (ook)
		ook = parser_int_check_n_get(srv, val);
	return (ook);
}
