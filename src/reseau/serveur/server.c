/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:32:12 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/05 19:45:44 by tbalea           ###   ########.fr       */
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

static void	fds_set(int socket, t_fds *fds)
{
	FD_SET(socket, &fds->rd);
	FD_SET(socket, &fds->wr);
	FD_SET(socket, &fds->ex);
}

static void	clear_and_set(t_fds *fds, t_server *srv)
{
	t_client	*clt;
	t_gfx		*gfx;

	FD_ZERO(&fds->rd);
	FD_ZERO(&fds->wr);
	FD_ZERO(&fds->ex);
	fds->max = srv->socket;
	fds_set(srv->socket, fds);
	clt = srv->clt;
	gfx = srv->gfx;
	while (clt)
	{
		fds->max = clt->socket > fds->max ? clt->socket : fds->max;
		if (clt->socket > 0)
			fds_set(clt->socket, fds);
		clt = clt->next;
	}
	while (gfx)
	{
		fds->max = gfx->socket > fds->max ? gfx->socket : fds->max;
		if (gfx->socket > 0)
			fds_set(gfx->socket, fds);
		gfx = gfx->next;
	}
	fds->max++;
}

//	TODO
//	*	check exception
//	*	check signal
//	*	close and free all
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
		recv_client(fds, srv, ret);
		send_client(fds, srv, ret);
		/*exception(fds, srv);*/
	}
	//close_server(srv, fds);
	return (ret);
}
