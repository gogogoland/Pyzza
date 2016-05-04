/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:32:12 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/04 15:42:21 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*error_msg[] =
{
	"Bad argument, usage: ./srv -p <port> -x <width> -y <height> -n <team> \
... <team> -c <nb> -t <t>",
	"Select error",
	"Malloc error"
};

static int	return_msg(const char *msg, int r)
{
	ft_putendl(msg);
	return r;
}

static t_client	*set_clients_list(t_server *srv)
{
	int			i;
	t_client    *new;
	t_client    *cur;

	if (srv->old_player_max != srv->player_max)
	{
		i = srv->old_player_max;
		while (i++ < srv->player_max)
		{
			cur = srv->clt;
			while (cur && cur->next)
				cur = cur->next;
			if (!(new = client_init()))
				return (NULL);
			if ((new->prev = cur))
				cur->next = new;
			else
				srv->clt = new;
		}
	}
	srv->old_player_max = srv->player_max;
	return (srv->clt);
}

static void	clear_and_set(t_fds *fds, t_server *srv)
{
	t_client	*cur;

	FD_ZERO(&fds->rd);
	FD_ZERO(&fds->wr);
	FD_ZERO(&fds->ex);
	fds->max = srv->socket;
	FD_SET(srv->socket, &fds->rd);
	FD_SET(srv->socket, &fds->wr);
	FD_SET(srv->socket, &fds->ex);
	cur = srv->clt;
	while (cur)
	{
		fds->max = cur->socket > fds->max ? cur->socket : fds->max;
		if (cur->socket > 0)
		{
			FD_SET(cur->socket, &fds->rd);
			FD_SET(cur->socket, &fds->wr);
			FD_SET(cur->socket, &fds->ex);
		}
		cur = cur->next;
	}
	fds->max++;
}

int			main(int argc, char **argv)
{
	t_server		*srv;
	t_fds			*fds;
	int				ret;

	fds = (t_fds *)malloc(sizeof(t_fds));
	ret = 0;
	if (!(srv = server_create(argc, argv)) || srv->socket < 0)
		return (return_msg(srv ? NULL : error_msg[0], srv ? srv->socket : -1));
	while (ret >= 0)
	{
		if (!(srv->clt = set_clients_list(srv)))
			return (-1);
		clear_and_set(fds, srv);
		if ((ret = select(fds->max, &fds->rd, &fds->wr, &fds->ex, NULL)) < 0)
			return (return_msg(error_msg[1], ret));
		else if (recv_client(fds, srv, ret))
			continue ;
		/*else if (send_client(fds, srv))
			continue ;
		exception(fds, srv);*/
	}
	//close_server(srv, fds);
	return (ret);
}
