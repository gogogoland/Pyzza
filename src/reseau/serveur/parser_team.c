/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_team.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/02 18:36:02 by croy              #+#    #+#             */
/*   Updated: 2016/10/02 20:08:14 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_parser_team_flag =
{
	"-n"
};

static const char	*g_parser_team_unauthorized_name =
{
	"GRAPHIC"
};

static const char	*g_parser_team_log[] =
{
	"[ERROR] : \"%s\" is not an authorized name for a team.\n",
	"[ERROR] : there no teams.\n",
	"[ERROR] : team memory allocation failed.\n"
};

static bool	parser_team_log(t_server *srv, int type, char *name)
{
	char	*log;

	if (asprintf(&log, g_parser_team_log[type], name))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
	return (false);
}

static int	parser_team_n_team(t_server *srv, char **av, int ac)
{
	int		i;
	int		no;
	int		n_team;

	i = 0;
	no = 0;
	n_team = 0;
	while (i < ac && strcmp(av[i], g_parser_team_flag))
		i++;
	while (++i < ac && av[i][0] != '-')
	{
		if (strcmp(av[i], g_parser_team_unauthorized_name))
			n_team++;
		else if (!no++)
			(parser_team_log(srv, 0, 0));
	}
	return (n_team);
}

bool		parser_team(int ac, char **av, t_server *srv)
{
	int		i;
	int		n_team;

	i = 0;
	n_team = parser_team_n_team(srv, av, ac);
	if (!n_team)
		return (parser_team_log(srv, 1, 0));
	if (!(srv->team = malloc(sizeof(char**) * (n_team + 1))))
		return (parser_team_log(srv, 2, 0));
	i = 0;
	srv->team[n_team] = NULL;
	while (i < ac && !strcmp(av[i], g_parser_team_flag))
		i++;
	while (++i < ac && av[i][0] != '-')
	{
		if (strcmp(av[i], g_parser_team_unauthorized_name))
			srv->team[--n_team] = strdup(av[i]);
	}
	return (true);
}
