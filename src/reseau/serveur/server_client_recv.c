/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client_recv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:38:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/03 18:52:47 by tbalea           ###   ########.fr       */
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
};

void		client_connect(int s, t_client *clt, t_fds *fds, t_server *srv)
{
	t_client	*new;
	t_client	lim;

	new = clt;
	while (new && new->socket != 0)
		new = new->next;
	if (!new)
	{
		lim.len = sizeof(struct sockaddr_in);
		if ((lim.socket = accept(s, (struct sockaddr *)&lim.sin, &lim.len)) < 0)
			return ;
		send(lim.socket, scr[0], strlen(scr[0]), 0);
		close(lim.socket);
		s = srv->player_max;
		printf(scr[1], inet_ntoa(lim.sin.sin_addr), ntohs(lim.sin.sin_port), s);
	}
	else if ((s = accept(s, (struct sockaddr *)&new->sin, &new->len)) < 0)
		printf(scr[2], s);
	else
	{
		new->socket = s;
		printf(scr[3], inet_ntoa(clt->sin.sin_addr) , ntohs(clt->sin.sin_port));
		fds->max = new->socket > fds->max-1 ? new->socket+1 : fds->max;
	}
}

void		client_command(int s, t_client *clt, t_fds *fds, t_server *srv)
{
	int			i;
	int			rd_len;
	t_client	*cur_clt;
	char		buffer[1024];

	cur_clt = clt;
	bzero(buffer, 1024);
	while (cur_clt && cur_clt->socket != s)
		cur_clt = cur_clt->next;
	if (!cur_clt || (rd_len = read(s, buffer, 1024)) < 0)
		return ;
	else if (rd_len == 0)
		client_zero(cur_clt, fds);
	else
		ring_recv(buffer, cur_clt->ring);
	return ;
}
