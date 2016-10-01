/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/01 13:22:08 by croy              #+#    #+#             */
/*   Updated: 2016/10/01 17:15:17 by croy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static const char	*g_flag[] = {
	"-p", "-x", "-y", "-c", "-t", "-b", "-n",
	"port", "width", "height", "Number of player", "time", "team",
};

int			digitcheck(char *str)
{
	int		i;

	i = 0;
	while (str[++i])
	{
		if (!isdigit(str[i]))
			return (0);
	}
	return (1);
}

int			word_after_flag(char **av, int size)
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

void		checkvalint(t_err val)
{
	if ((val.flag_int[0] < P_MIN && val.flag_int[0] > P_MAX) &&\
		(val.flag_int[1] < X_MIN && val.flag_int[1] > X_MAX) &&\
		(val.flag_int[2] < Y_MIN && val.flag_int[2] > Y_MAX) &&\
		(val.flag_int[3] < C_MIN && val.flag_int[3] > C_MAX) &&\
		(val.flag_int[4] < T_MIN && val.flag_int[4] > T_MAX))
	{
		printf("[ERROR] : your value(s) is/are not correct.\n");
		exit(-1);
	}
}

void		integerserror(int check[6])
{
	int		i;
	int		ex;
	t_err	value;

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
	if (ex > 0)
		exit(ex);
}

void		checkoutsiderflag(char **argv, int ac)
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
	if (err > 0)
		exit(-1);
}

void		parsingint(int argc, char **argv, int *check, t_err *val)
{
	int		j;
	int		i;

	j = -1;
	checkoutsiderflag(argv, argc);
	while (++j < 6)
	{
		i = 0;
		while (++i < (argc))
		{
			if (!strcmp(argv[i], g_flag[j]) && i < (argc - 1) &&\
					digitcheck(argv[i + 1]))
			{
				check[j] += 1;
				if (check[j] == 1)
					val->flag_int[j] = atoi(argv[i + 1]);
			}
		}
	}
	checkvalint(*val);
}

void		add_team(char **av, int ac, t_err *val)
{
	int		i;
	int		word;

	i = 0;
	word = 0;
	val->team = malloc(sizeof(char**) * word_after_flag(av, ac));
	while (++i < ac)
	{
		if (!strcmp(av[i], "-n"))
		{
			while (++i < ac && av[i][0] != '-')
			{
				val->team[word] = strdup(av[i]);
				word++;
			}
			val->team[word] = NULL;
		}
	}
}

t_err		parsingflags(char **argv, int argc)
{
	int		check[7];
	int		i;
	int		j;
	t_err	value;

	i = -1;
	while (++i < 7)
	{
		check[i] = 0;
		value.flag_int[i] = 0;
	}
	parsingint(argc, argv, check, &value);
	i = -1;
	while (++i < 6)
		printf("%d\n", value.flag_int[i]);
	integerserror(check);
	add_team(argv, argc, &value);
	return (value);
}

int			main(int ac, char **av)
{
	printf("%d\n", word_after_flag(av, ac));
	if (ac < 14)
	{
		printf(ERR_ARG);
		exit(-1);
	}
	parsingflags(av, ac);
	return (0);
}
