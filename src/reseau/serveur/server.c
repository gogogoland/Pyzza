/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:32:12 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 17:36:45 by tbalea           ###   ########.fr       */
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

ft_putendl("server.c		set_clt_lst	0");
	if (srv->old_player_max != srv->player_max)
	{
ft_putendl("server.c		set_clt_lst	1");
		i = srv->old_player_max;
		while (i++ < srv->player_max)
		{
ft_putendl("server.c		set_clt_lst	2");
			cur = srv->clt;
			while (cur && cur->next)
				cur = cur->next;
ft_putendl("server.c		set_clt_lst	3");
			if (!(new = client_init()))
				return (NULL);
ft_putendl("server.c		set_clt_lst	4");
			if ((new->prev = cur))
				cur->next = new;
			else
				srv->clt = new;
ft_putendl("server.c		set_clt_lst	5");
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

ft_putendl("server_");
ft_putendl("server.c		clear_&_set	0");
	FD_ZERO(&fds->rd);
	FD_ZERO(&fds->wr);
	FD_ZERO(&fds->ex);
	fds->max = srv->socket;
	fds_set(srv->socket, fds);
	clt = srv->clt;
	gfx = srv->gfx;
ft_putendl("server.c		clear_&_set	1");
	while (clt)
	{
		fds->max = clt->socket > fds->max ? clt->socket : fds->max;
		if (clt->socket > 0)
			fds_set(clt->socket, fds);
		clt = clt->next;
	}
ft_putendl("server.c		clear_&_set	2");
	while (gfx)
	{
ft_putendl("server.c		clear_&_set	3");
		fds->max = gfx && (gfx->socket > fds->max) ? gfx->socket : fds->max;
		if (gfx && gfx->socket > 0)
			fds_set(gfx->socket, fds);
		gfx = gfx ? gfx->next : NULL;
	}
ft_putendl("server.c		clear_&_set	4");
	fds->max++;
}

//	TODO
//	*	check exception
//	*	check signal
//	*	close and free all
//	*	add sleep for connection and disconnect
//	*	exit player
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
ft_putendl("server.c 		main			0");
		if (!(srv->clt = set_clients_list(srv)))
			return (-1);
ft_putendl("server.c		main			1");
		clear_and_set(fds, srv);
ft_putendl("server.c 		main			2");
		if ((ret = select(fds->max, &fds->rd, &fds->wr, &fds->ex, NULL)) < 0)
			return (return_msg(error_msg[1], ret));
ft_putendl("server.c 		main			3");
		recv_client(fds, srv, ret);
ft_putendl("server.c 		main			4");
		send_client(fds, srv, ret);
		/*exception(fds, srv);*/
	}
	//close_server(srv, fds);
	return (ret);
}
