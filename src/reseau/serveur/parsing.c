/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/01 13:22:08 by croy              #+#    #+#             */
/*   Updated: 2016/10/01 21:47:14 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_flag[] =
{
	"-p",
	"-x",
	"-y",
	"-c",
	"-t",
	"-b",
	"-n",
	"port",
	"width",
	"height",
	"Number of player",
	"time",
	"team"
};

static int	number_of_team(char **av, int size)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (++i < size)
	{
		if (!strcmp(av[i], "-n"))
		{
			while (++i < size && av[i][0] != '-')
				count++;
		}
	}
	return (count);
}

static bool	integers_error(int check[6])
{
	int		i;
	int		ex;

	i = -1;
	ex = 0;
	while (++i < 5)
	{
		if (check[i] != 1)
		{
			check[i] < 1 ?\
				printf("[ERROR] : %s is missing.\n", g_flag[i]) :\
				printf("[ERROR] : multiple usage of %s.\n", g_flag[i]);
			ex++;
		}
	}
	return (ex ? false : true);
}

static bool	check_outsider_flag(char **argv, int ac)
{
	int		i;
	int		j;
	int		tir;
	int		err;

	i = 0;
	j = 0;
	while (++i < (ac))
	{
		err = 0;
		j = -1;
		while (++j < 6)
		{
			if (argv[i][0] == '-' && strcmp(argv[i], g_flag[j]) == 0)
				tir++;
		}
		if (argv[i][0] == '-' && tir == 0)
		{
			err += 1;
			printf("[ERROR] : '%s' is not supported as flag.\n", argv[i]);
		}
	}
	return (err ? false : true);
}

static int	*parsing_int(int argc, char **argv, int *check, int *val)
{
	int		j;
	int		i;

	j = -1;
	if (check_outsider_flag(argv, argc) < 0)
		return (NULL);
	while (++j < 6)
	{
		i = 0;
		while (++i < (argc))
		{
			if (!strcmp(argv[i], g_flag[j]) && i < (argc - 1) &&
					ft_strisdigit(argv[i + 1]))
				val[j] = (check[j] += 1) == 1 ? atoi(argv[i + 1]) : val[j];
		}
	}
	if (val[0] <= P_MIN || val[0] >= P_MAX || val[1] <= X_MIN || val[1] >= X_MAX
			|| val[2] <= Y_MIN || val[2] >= Y_MAX || val[3] <= C_MIN
			|| val[3] >= C_MAX || val[4] <= TIME_MIN || val[4] >= TIME_MAX)
	{
		printf("[ERROR] : your value(s) is/are not correct.\n");
		return (NULL);
	}
	return (val);
}

static bool	add_teams(char **av, int ac, int *val, t_server *srv)
{
	int		i;
	int		j;
	int		word;
	char	**team;

	i = 0;
	if (!(word = number_of_team(av, ac))
			|| !(srv->team = (char **)malloc(sizeof(char *) * word)))
		return (false);
	word = 0;
	while (++i < ac)
	{
		if (!strcmp(av[i], "-n"))
		{
			while (++i < ac && av[i][0] != '-')
			{
				srv->team[word] = strdup(av[i]);
				word++;
			}
			srv->team[word] = NULL;
		}
	}
	return (true);
}

void		parser(int argc, char **argv, t_server *srv)
{
	int		check[7];
	int		i;
	int		value[7];

	i = -1;
	while (++i < 7)
	{
		check[i] = 0;
		value[i] = 0;
	}
	if (!parsing_int(argc, argv, check, value) || !integers_error(check)
			|| !add_teams(argv, argc, value, srv))
		return ;
	srv->port = value[0];
	srv->plateau.x = value[1];
	srv->plateau.y = value[2];
	srv->player_max = value[3];
	srv->time = value[4];
	srv->bonus_fork = value[5] ? true : false;
	srv->socket = 0;
}
