/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:32:12 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/27 21:07:55 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*error_msg[] =
{
	"Bad argument, usage: ./serveur -p <port> -x <width> -y <height> -n <team> \
... <team> -c <nb> -t <t>",
	"Select error"
};

static int	*set_clients_sockets_list(t_server *srv, int *clients_sockets)
{
	int	i;
	int	*new;

	i = -1;
	if (clients_sockets == NULL)
	{
		if (!(clients_sockets = (int *)malloc(srv->player_max * sizeof(int))))
			return (NULL);
		while (++i < srv->player_max)
			clients_sockets[i] = 0;
	}
	else if (srv->old_player_max != srv->player_max)
	{
		if (!(new = (int *)malloc(srv->player_max * sizeof(int))))
			return (NULL);
		while (++i < srv->old_player_max)
			new[i] = clients_sockets[i];
		i -= 1;
		while (++i < srv->player_max)
			new[i] = 0;
		free(clients_sockets);
		return (new);
	}
	srv->old_player_max = srv->player_max;
	return (clients_sockets);
}

static int	clear_and_set(t_fds *fds, t_server *srv, int *clients_sockets)
{
	int	i;
	int	max_sock;

	i = -1;
	FD_ZERO(&fds->rd_fds);
	FD_ZERO(&fds->wr_fds);
	FD_ZERO(&fds->ex_fds);
	max_sock = srv->socket;
	FD_SET(srv->socket, &fds->rd_fds);
	FD_SET(srv->socket, &fds->wr_fds);
	FD_SET(srv->socket, &fds->ex_fds);
	while (++i < srv->player_max)
	{
		if (clients_sockets[i] > max_sock)
			max_sock = clients_sockets[i];
		FD_SET(clients_sockets[i], &fds->rd_fds);
		FD_SET(clients_sockets[i], &fds->wr_fds);
		FD_SET(clients_sockets[i], &fds->ex_fds);
	}
	return (max_sock + 1);
}

int			main(int argc, char **argv)
{
	t_server	*srv;
	t_fds		*fds;
	int			*clients_sockets;
	int			ret;

	clients_sockets = NULL;
	fds = (t_fds *)malloc(sizeof(t_fds));
	ret = 0;
	if (!(srv = server_create(argc, argv)))
		return (0);
	if (srv->socket == -1)
		ft_putendl(error_msg[0]);
	if (srv->socket < 0)
		return (0);
	while (ret >= 0)
	{
		if (!(clients_sockets = set_clients_sockets_list(srv, clients_sockets)))
			return (-1);
		ret = clear_and_set(fds, srv, clients_sockets);
		ret = select(ret, &fds->rd_fds, &fds->wr_fds, &fds->ex_fds, NULL);
		if (ret < 0)
			ft_putendl(error_msg[1]);
	}
	return (1);
}
