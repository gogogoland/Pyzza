/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_recv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:38:10 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 16:02:16 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	client_concatenate(t_server *srv, char *cmd, t_ring *ring, int who)
{
	t_cmd	cur;

	cur.beg = 0;
	cur.end = 0;
	cur.cmd = cmd;
	while (cur.cmd[cur.end] != '\0')
	{
		if (cur.cmd[cur.end] == '\n')
		{
			ring_recv(srv, cur, ring, who);
			cur.beg = cur.end + 1;
		}
		cur.end++;
	}
	if (cur.end > 1 + cur.beg)
		ring_recv(srv, cur, ring, who);
}

static void	client_command(t_fds *fds, t_server *srv, int s)
{
	t_client	*clt;
	t_gfx		*gfx;
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
		!clt ? graphe_kill(srv, gfx, fds, 0) : command_death(fds, srv, clt, 0);
	else
	{
		client_concatenate(srv, buffer, !clt ? gfx->ring : clt->ring,
					!clt ? gfx->socket * -1 : clt->name);
	}
}

static void	client_connect(t_fds *fds, t_server *srv, int s)
{
	t_gfx	*cur;
	t_gfx	*new;

	cur = srv->gfx;
	while (cur && cur->next)
		cur = cur->next;
	if (!(new = graphe_news(srv, cur, fds, s)))
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
