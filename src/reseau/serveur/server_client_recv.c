/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client_recv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:38:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/04 16:59:56 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*scr[] =
{
		"Reached client limit's.\n",
		"Client from ip %s, port %d tried to connect, but limit of %d client is\
 reached.\n",
		"Error accept %d client\n",
		"New client from ip %s, port %d.\n",
		"Graphic client tried to connect, but malloc failed.\n"
};

static void	player_connect(t_fds *fds, t_server *srv, t_tmp tmp)
{
	t_client	*new;
	t_client	lim;

	new = srv->clt;
	while (new && new->socket != 0)
		new = new->next;
	if (!new)
	{
		send(tmp.socket, scr[0], strlen(scr[0]), 0);
		close(tmp.socket);
		printf(scr[1], inet_ntoa(lim.sin.sin_addr), ntohs(lim.sin.sin_port), \
				srv->player_max);
	}
	else
	{
		new->socket = tmp.socket;
		new->sin = tmp.sin;
		new->len = tmp.len;
		printf(scr[3], inet_ntoa(new->sin.sin_addr) , ntohs(new->sin.sin_port));
		fds->max = new->socket > fds->max-1 ? new->socket+1 : fds->max;
	}
}

static void	player_command(t_fds *fds, t_server *srv, int s)
{
	int			i;
	int			rd_len;
	t_client	*clt;
	char		buffer[1024];

	clt = srv->clt;
	bzero(buffer, 1024);
	while (clt && clt->socket != s)
		clt = clt->next;
	if (!clt || (rd_len = read(s, buffer, 1024)) < 0)
		return ;
	else if (rd_len == 0)
		client_zero(clt, fds);
	else
		ring_recv(buffer, clt->ring);
	return ;
}

static void	graphe_connect(t_fds *fds, t_server *srv, t_tmp tmp)
{
	t_gfx	*cur;
	t_gfx	*new;

	cur = srv->gfx;
	while (cur && cur->next)
		cur = cur->next;
	if (!(new = graphe_init(cur, tmp)))
		printf("%s", scr[4]);
	else if (cur)
		cur->next = new;
	else
		srv->gfx = new;
}

static void	client_connect(t_fds *fds, t_server *srv, int s)
{
	t_tmp	tmp;

	tmp.len = sizeof(struct sockaddr_in);
	if ((tmp.socket = accept(s, (struct sockaddr *)&tmp.sin, &tmp.len)) < 0)
		printf(scr[2], s);
	else
		player_connect(fds, srv, tmp);
}

bool		recv_client(t_fds *fds, t_server *srv, int ret)
{
	int	s;
	
	s = -1;
	if (ret <= 0)
		return true;
	while (++s < fds->max)
	{
		if (FD_ISSET(s, &fds->rd))
		{
			if (s == srv->socket)
				client_connect(fds, srv, s);
			else
				player_command(fds, srv, s);
			return true;
		}
	}
	return false;
}
