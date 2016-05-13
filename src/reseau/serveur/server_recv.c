/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_recv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:38:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/12 18:48:05 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*scr[] =
{
		"Error accept %d client\n",
		"New client from ip %s, port %d.\n",
		"Client tried to connect, but malloc failed.\n",
		"Client discnnected , ip %s , port %d.\n"
};

static void client_command(t_fds *fds, t_server *srv, int s)
{
	t_client	*clt;
	t_gfx		*gfx;
	int			i;
	int			rd_len;
	char		buffer[1024];

ft_putendl("server_recv.c	client_cmd	0");
	clt = srv->clt;
	gfx = srv->gfx;
	bzero(buffer, 1024);
ft_putendl("server_recv.c	client_cmd	1");
	if ((rd_len = recv(s, buffer, 1024, 0)) < 0)
		return ;
ft_putendl("server_recv.c	client_cmd	2");
	while (clt && clt->socket != s)
		clt = clt->next;
ft_putendl("server_recv.c	client_cmd	3");
	while (!clt && gfx && gfx->socket != s)
		gfx = gfx->next;
ft_putendl("server_recv.c	client_cmd	4");
	if (!gfx && !clt)
		return ;
ft_putendl("server_recv.c	client_cmd	5");
	if (rd_len == 0)
		!clt ? graphe_kill(gfx, fds, false) : client_zero(clt, fds);
	else
		ring_recv(buffer, !clt ? gfx->ring : clt->ring);
ft_putendl("server_recv.c	client_cmd	6");
}

static void	client_connect(t_fds *fds, t_server *srv, int s)
{
	t_gfx	*cur;
	t_gfx	*new;

ft_putendl("server_recv.c	client_co	0");
	cur = srv->gfx;
	while (cur && cur->next)
		cur = cur->next;
ft_putendl("server_recv.c	client_co	1");
	if (!(new = graphe_init(cur, fds, s)))
		return ;
	else if (cur)
		cur->next = new;
	else
		srv->gfx = new;
ft_putendl("server_recv.c	client_co	2");
}

bool		recv_client(t_fds *fds, t_server *srv, int ret)
{
	int	s;
	
ft_putendl("server_recv.c	client_recv	0");
	s = -1;
	if (ret <= 0)
		return true;
ft_putendl("server_recv.c	client_recv	1");
	while (++s < fds->max)
	{
ft_putendl("server_recv.c	client_recv	2");
		if (FD_ISSET(s, &fds->rd))
		{
ft_putendl("server_recv.c	client_recv	3");
			if (s == srv->socket)
				client_connect(fds, srv, s);
			else
				client_command(fds, srv, s);
ft_putendl("server_recv.c	client_recv	4");
			return true;
		}
	}
ft_putendl("server_recv.c	client_recv	5");
	return false;
}
