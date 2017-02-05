/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:51:14 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/05 18:57:29 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char			*g_error[] =
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
	"Unchecked time divisor"
};

static t_server	*return_msg(const char *msg, int ret, t_server *srv)
{
	server_log(srv, msg);
	srv->socket = ret;
	return (srv);
}

static t_server	*server_init_data(int ac, char **av)
{
	t_server	*srv;

	srv = NULL;
	if (ac < 13 || !(srv = (t_server *)malloc(sizeof(t_server))))
		return (srv);
	srv->lvl = 1;
	srv->egg = 0;
	srv->name = 0;
	srv->gfx = NULL;
	srv->clt = NULL;
	srv->team = NULL;
	srv->socket = -1;
	srv->old_player_max = 0;
	if (!parser(ac, av, srv))
		return (srv);
	srv->socket = 0;
	return (init_map(srv));
}

t_server		*server_create(int argc, char **argv)
{
	int					sso;
	t_server			*srv;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	sso = 1;
	if (!(srv = server_init_data(argc, argv)) || srv->socket == -1)
		return (srv);
	if (!(proto = getprotobyname("tcp")))
		return (return_msg(g_error[12], -1, srv));
	if ((srv->socket = socket(PF_INET, SOCK_STREAM, proto->p_proto)) < 0)
		return (return_msg(g_error[13], srv->socket, srv));
	if (setsockopt(srv->socket, SOL_SOCKET,
				SO_REUSEADDR, (char *)&sso, sizeof(sso)) < 0)
		return (return_msg(g_error[14], -1, srv));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(srv->port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(srv->socket, (const struct sockaddr*)&sin, sizeof(sin)) == -1)
		return (return_msg(g_error[15], -1, srv));
	else if (listen(srv->socket, srv->player_max) < 0 && (srv->socket = -4) < 0)
		return (return_msg(g_error[16], -1, srv));
	srv->gfx = graphe_init(srv);
	return (srv);
}
