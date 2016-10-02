/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/01 13:22:08 by croy              #+#    #+#             */
/*   Updated: 2016/10/02 20:06:16 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_parser_check_flag[] =
{
	"-p",
	"-x",
	"-y",
	"-c",
	"-t",
	"-n",
	"-b",
};

static const char	*g_parser_check_log[] =
{
	"[ERROR] : '%s' is not supported as flag.\n",
	"[ERROR] : multiple usage of '%s'.\n",
	"[ERROR] : '%s' is missing.\n"
};

static void	parser_check_log(t_server *srv, int type, const char *flag)
{
	char	*log;

	if (asprintf(&log, g_parser_check_log[type], flag))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
	return ;
}

static int	parser_check_flags_number(int ac, char **av, t_server *srv)
{
	int	i;
	int	j;
	int	n_flag[7];

	i = 8;
	while (--i > 0)
		n_flag[i - 1] = 0;
	while (++i < ac && (j = -1))
	{
		while (av[i][0] == '-' && ++j < 7)
			n_flag[j] += strcmp(av[i], g_parser_check_flag[j]) ? 0 : 1;
	}
	j = -1;
	while (++j < 6)
	{
		if (n_flag[j] != 1)
		{
			i = 0;
			parser_check_log(srv, n_flag[j] ? 1 : 2, g_parser_check_flag[j]);
		}
	}
	i = i ? n_flag[j] * 2 - 1 : 0;
	return (i);
}

static bool	parser_check_flags_unknow(int ac, char **av, t_server *srv)
{
	int		i;
	int		j;
	int		flag;
	int		err;

	i = 0;
	err = 0;
	while (++i < ac)
	{
		flag = 1;
		j = -1;
		while (++j < 7 && flag)
			flag = (av[i][0] != '-'
					|| strcmp(av[i], g_parser_check_flag[j]) == 0) ? 0 : 1;
		if (av[i][0] == '-' && flag)
			parser_check_log(srv, 0, av[i]);
		err += (av[i][0] == '-') ? flag : 0;
	}
	return (err > 0 ? false : true);
}

bool	parser(int ac, char **av, t_server *srv)
{
	int	bonus;

	if (!parser_check_flags_unknow(ac, av, srv))
		return (false);
	if (!(bonus = parser_check_flags_number(ac, av, srv)))
		return (false);
	if (!parser_int(ac, av, srv))
		return (false);
	if (!parser_team(ac, av, srv))
		return (false);
	srv->bonus_fork = bonus > 0 ? 1 : 0;
	srv->socket = 1;
	return (true);
}
