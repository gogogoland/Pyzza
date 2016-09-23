/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_recv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:38:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/19 16:12:09 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_scr[] =
{
		"Error accept %d client\n",
		"New client from ip %s, port %d.\n",
		"Client tried to connect, but malloc failed.\n",
		"Client disconnected , ip %s , port %d.\n"
};

static void client_command(t_fds *fds, t_server *srv, int s)
{
	t_client	*clt;
	t_gfx		*gfx;
	int			i;
	int			rd_len;
	char		buffer[1024];

	clt = srv->clt;
	gfx = srv->gfx;
	bzero(buffer, 1024);
	if ((rd_len = recv(s, buffer, 1024, 0)) < 0)
		return ;
	while (clt && clt->socket != s)
		clt = clt->next;
	while (!clt && gfx && gfx->socket != s)
		gfx = gfx->next;
	if (!gfx && !clt)
		return ;
	if (rd_len == 0)
		!clt ? graphe_kill(gfx, fds, false) : command_death(fds, srv, clt, NULL);
	else
		ring_recv(buffer, !clt ? gfx->ring : clt->ring);
}

static void	client_connect(t_fds *fds, t_server *srv, int s)
{
	t_gfx	*cur;
	t_gfx	*new;

	cur = srv->gfx;
	while (cur && cur->next)
		cur = cur->next;
	if (!(new = graphe_news(cur, fds, s)))
		return ;
	else if (cur)
		cur->next = new;
	else
		srv->gfx = new;
}

bool		recv_client(t_fds *fds, t_server *srv, int ret)
{
	int	s;

	s = -1;
	if (ret <= 0)
		return (true);
	while (++s < fds->max)
	{
		if (FD_ISSET(s, &fds->rd))
		{
			if (s == srv->socket)
				client_connect(fds, srv, s);
			else
				client_command(fds, srv, s);
			return (true);
		}
	}
	return (false);
}
