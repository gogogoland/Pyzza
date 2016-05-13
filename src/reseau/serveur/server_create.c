/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:51:14 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/12 16:50:44 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char			*c_arg[] =
{
	"-p",
	"-x",
	"-y",
	"-n",
	"-c",
	"-t"
};

static const char			*error[] =
{
	"Wrong port number",
	"Wrong data for World's weight",
	"Wrong data for World's height",
	"Mallocing failed :)",
	"Wrong value for number of authorized client",
	"Wrong value for time divisor",
	"Missing port number",
	"Missing weight value for the World",
	"Missing height value for the World",
	"Missing team name",
	"Missing number of authorized client",
	"Missing time divisor",
	"Get Protocol error",
	"Socket error",
	"Setsockopt error",
	"Bind error",
	"Listen error",
	"Unchecked port number",
	"Unchecked weight value for the World",
	"Unchecked height value for the World",
	"Unchecked team name",
	"Unchecked number of authorized client",
	"Unchecked time divisor",
};

static t_server	*return_msg(const char *msg, int ret, t_server *srv)
{
	ft_putendl(msg);
	srv->socket = ret;
	return (srv);
}

static int		get_data(t_server *srv, int state, char *arg, int isdone)
{
	int	e;
	int	p;

	p = 1;
	e = 0;
ft_putendl("server_create.c	get_data	0");
	if (state == 0 && (srv->port = ft_atoi(arg)) <= 0)
		e = 0;
	else if (state == 1 && (srv->plateau.x = atoi(arg)) <= 0)
		e = 1;
	else if (state == 2 && (srv->plateau.y = atoi(arg)) <= 0)
		e = 2;
	else if (state == 3 && !(srv->team = ft_txtadd(srv->team, arg)))
		e = 3;
	else if (state == 4 && (srv->player_max = atoi(arg)) <= 2)
		e = 4;
	else if (state == 5 && (srv->time = atoi(arg)) <= 0)
		e = 5;
ft_putendl("server_create.c	get_data	1");
	while (state-- > 0)
		p *= 10;
ft_putendl("server_create.c	get_data	2");
	if ((isdone / p) % 10 == 1)
		p = 0;
ft_putendl("server_create.c	get_data	3");
	if (e == 0)
		return (p);
ft_putendl("server_create.c	get_data	4");
	ft_putendl(error[e]);
	return (0);
}

static t_server	*check_data(t_server *srv, int isdone)
{
	int			e;

ft_putendl("server_create.c	check_data	0");
	srv->gfx = NULL;
	srv->clt = NULL;
	srv->socket = 0;
	srv->old_player_max = 0;
	init_map(srv);
ft_putendl("server_create.c	check_data	1");
	if (isdone == 111111)
		return (srv);
ft_putendl("server_create.c	check_data	2");
	srv->socket = -1;
	ft_tabdel(srv->team);
	e = 6;
	while (isdone > 0)
	{
ft_putendl("server_create.c	check_data	3");
		if (isdone % 10 == 0)
			ft_putendl(error[e]);
		isdone /= 10;
		e++;
	}
ft_putendl("server_create.c	check_data	4");
	while (++e < 12)
		ft_putendl(error[e + 11]);
ft_putendl("server_create.c	check_data	5");
	return (srv);
}

//	TODO
//	*	check if next argument begin with '-' character
static t_server	*server_init_data(int argc, char **argv)
{
	int 		i;
	int			arg;
	int			isdone;
	int			state;
	t_server	*srv;

ft_putendl("server_create.c	init_data	0");
	srv = NULL;
	i = 0;
	state = -1;
	isdone = 0;
	if (argc < 13 || !(srv = (t_server *)malloc(sizeof(t_server))))
		return (srv);
ft_putendl("server_create.c	init_data	1");
	srv->team = NULL;
	while (++i < argc)
	{
ft_putendl("server_create.c	init_data	2");
		arg = 0;
		while (arg < 6 && ft_strcmp(c_arg[arg], argv[i]) != 0)
			arg++;
ft_putendl("server_create.c	init_data	3");
		if (arg != 6 && (state = arg) == arg)
			i++;
ft_putendl("server_create.c	init_data	4");
		if ((arg == 6 && state != 3)
				|| !(isdone += get_data(srv, state, argv[i], isdone)))
			return (check_data(srv, isdone));
ft_putendl("server_create.c	init_data	5");
	}
ft_putendl("server_create.c	init_data	6");
	return (check_data(srv, isdone));
}

/*static char	*tcp(void)
{
	char	*tcp_str;

	if (!(tcp_str = (char *)malloc(4 * sizeof(char))))
		return (NULL);
	tcp_str = "tcp\0";
	ft_putendl(tcp_str);
	return (tcp_str);
}*/

//	TODO
//	*	IPv6
//	*	TCP error
t_server	*server_create(int argc, char **argv)
{
	int 				sso;
	t_server			*srv;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	//char	*tcp;
	//tcp = strdup("tcp");

	sso = 1;
ft_putendl("server_create.c	create	0");
	if (!(srv = server_init_data(argc, argv)) || srv->socket == -1)
		return (srv);
ft_putendl("server_create.c	create	1");
	if (!(proto = getprotobyname("tcp")))
		return (return_msg(error[12], -1, srv));
ft_putendl("server_create.c	create	2");
	if ((srv->socket = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		return (return_msg(error[13], srv->socket, srv));
ft_putendl("server_create.c	create	3");
	if (setsockopt(srv->socket, SOL_SOCKET, 
				SO_REUSEADDR, (char *)&sso, sizeof(sso)) < 0)
		return (return_msg(error[14], -1, srv));
ft_putendl("server_create.c	create	4");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(srv->port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
ft_putendl("server_create.c	create	5");
	if (bind(srv->socket, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
		return (return_msg(error[15], -1, srv));
	else if (listen(srv->socket, srv->player_max) < 0 && (srv->socket = -4) < 0)
		return (return_msg(error[16], -1, srv));
ft_putendl("server_create.c	create	6");
	return (srv);
}
