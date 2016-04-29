/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:32:12 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/29 20:14:42 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*error_msg[] =
{
	"Bad argument, usage: ./serveur -p <port> -x <width> -y <height> -n <team> \
... <team> -c <nb> -t <t>",
	"Select error",
	"Malloc error"
};

static int	return_msg(const char *msg, int r)
{
	ft_putendl(msg);
	return r;
}

static bool	set_clients_list(t_server *srv, t_client *clients)
{
	int			i;
	t_client    *new;
	t_client    *cur;

	if (srv->old_player_max != srv->player_max)
	{
		i = srv->old_player_max;
		while (i++ < srv->player_max)
		{
			cur = clients;
			while (cur && cur->next)
			cur = cur->next;
			if (!(new = (t_client *)malloc(sizeof(t_client))))
				return (return_msg(error_msg[2], 0));
			new->socket = 0;
			new->next = NULL;
			if ((new->prev = cur))
				cur->next = new;
			else
				clients = new;
		}
	}
	srv->old_player_max = srv->player_max;
	return true;
}

static void	clear_and_set(t_fds *fds, t_server *srv, t_client *clients)
{
	int			i;
	int			max_socket;
	t_client	*cur;

	i = -1;
	FD_ZERO(&fds->rd);
	FD_ZERO(&fds->wr);
	FD_ZERO(&fds->ex);
	fds->max = srv->socket;
	FD_SET(srv->socket, &fds->rd);
	FD_SET(srv->socket, &fds->wr);
	FD_SET(srv->socket, &fds->ex);
	cur = clients;
	while (cur)
	{
		if (cur->socket > fds->max)
			fds->max = cur->socket;
		FD_SET(cur->socket, &fds->rd);
		FD_SET(cur->socket, &fds->wr);
		FD_SET(cur->socket, &fds->ex);
		cur = cur->next;
	}
	fds->max++;
}

static bool	recv_client(t_client *clt, t_fds *fds, t_server *srv)
{
	int			s;
	int			new_sock;
	t_client	*new_clt;

	s = -1;
	while (++s < srv->player_max)
	{
		if (FD_ISSET(s, &fds->rd))
		{
			if (s == srv->socket)
				client_connect(s, clt, fds, srv);
			/*else
				client_command(s, clt, fds, srv);*/
			return true;
		}
	}
	return false;
}

int			main(int argc, char **argv)
{
	t_server	*srv;
	t_fds		*fds;
	t_client	*clients;
	int			ret;

	clients = NULL;
	fds = (t_fds *)malloc(sizeof(t_fds));
	ret = 0;
	if (!(srv = server_create(argc, argv)) || srv->socket < 0)
		return (return_msg(error_msg[0], srv->socket));
	while (ret >= 0)
	{
		if (!set_clients_list(srv, clients))
			return (-1);
		clear_and_set(fds, srv, clients);
		if ((ret = select(fds->max, &fds->rd, &fds->wr, &fds->ex, NULL)) < 0)
			return (return_msg(error_msg[1], ret));
		else if (recv_client(clients, fds, srv))
			continue ;
		/*else if (send_client(clients, fds, srv))
			continue ;
		exception(clients, fds, srv);*/
	}
	//close_server(srv, fds, clients);
	return (1);
}
