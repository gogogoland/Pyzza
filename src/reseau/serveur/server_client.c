/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:38:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/03 14:38:29 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*msg[] =
{
		"Reached client limit's.\n",
		"Client from ip %s, port %d tried to connect, but limit of %d client is\
 reached.\n",
		"Error accept %d client\n",
		"New client from ip %s, port %d.\n",
		"Client disconnected , ip %s , port %d.\n"
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
		send(lim.socket, msg[0], strlen(msg[0]), 0);
		close(lim.socket);
		s = srv->player_max;
		printf(msg[1], inet_ntoa(lim.sin.sin_addr), ntohs(lim.sin.sin_port), s);
	}
	else if ((s = accept(s, (struct sockaddr *)&new->sin, &new->len)) < 0)
		printf(msg[2], s);
	else
	{
		new->socket = s;
		printf(msg[3], inet_ntoa(clt->sin.sin_addr) , ntohs(clt->sin.sin_port));
		fds->max = new->socket > fds->max-1 ? new->socket+1 : fds->max;
	}
}

static void	client_close(t_client *clt, t_fds *fds)
{
	getpeername(clt->socket, (struct sockaddr*)&clt->sin, \
			(socklen_t*)&clt->len);
	printf(msg[4], inet_ntoa(clt->sin.sin_addr) , ntohs(clt->sin.sin_port));
	close(clt->socket);
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	clt->socket = 0;
}

void		client_command(int s, t_client *clt, t_fds *fds, t_server *srv)
{
	int			rd_len;
	t_client	*cur_clt;
	char		buffer[1024];

	cur_clt = clt;
	bzero(buffer, 1024);
	while (cur_clt && cur_clt->socket != s)
		cur_clt = cur_clt->next;
	if (!cur_clt || (rd_len = read(s, buffer, 1024)) < 0)
		return ;
	if (rd_len == 0)
		client_close(cur_clt, fds);
	ft_putstr(buffer);
	return ;
}
