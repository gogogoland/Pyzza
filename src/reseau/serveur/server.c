/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:32:12 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/27 20:07:33 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*bad_arg =
{
	"Bad argument, usage: ./serveur -p <port> -x <width> -y <height> -n <team>\
... <team> -c <nb> -t <t>"
};

static int	clear_and_set(t_fds *fds, t_server *srv)
{
	int	i;
	int	max_sock;

	i = -1;
	FD_ZERO(fds->rd_fds);
	FD_ZERO(fds->wr_fds);
	FD_ZERO(fds->ex_fds);
	max_sock = srv->socket
	FD_SET(srv->socket, fds->rd_fds);
	FD_SET(srv->socket, fds->wr_fds);
	FD_SET(srv->socket, fds->ex_fds);
	while (++i < srv->player_max)
	{
		if ( > max_sock)
			max_sock = ;
		FD_SET(, fds->rd_fds);
		FD_SET(, fds->wr_fds);
		FD_SET(, fds->ex_fds);
	}
	return (max_sock + 1);
}

int			main(int argc, char **argv)
{
	t_server	*srv;
	t_fds		*fds;
	int			max_and_act;

	max_and_act = 0;
	if (!(srv = server_create(argc, argv)))
		return (0);
	if (srv->socket == -1)
		ft_putendl(bad_arg);
	if (srv->socket < 0)
		return (0);
	while (max_and_act >= 0)
	{
		max_and_act = clear_and_set(fds, srv);
		max_and_act = select(max, fds->rd_fds, fds->wr_fds, ex_fds);
		if (max_and_act < 0)
			ft_putendl("Select error.");
	}
	return (1);
}
