/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 16:18:55 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/05 13:53:45 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_client	*client_init(void)
{
	t_client	*clt;

	if (!(clt = (t_client *)malloc(sizeof(t_client))))
		return (clt);
	clt->socket = 0;
	clt->len = sizeof(struct sockaddr_in);
	clt->ring = ring_init(10);
	clt->next = NULL;
	clt->pos.x = 0;
	clt->pos.y = 0;
	return (clt);
}

void		client_kill(t_client *clt, t_fds *fds)
{
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	ring_kill(clt->ring);
	close(clt->socket);
	clt->socket = 0;
	clt->pos.x = 0;
	clt->pos.y = 0;
	free(clt);
	clt = NULL;
}

void		client_zero(t_client *clt, t_fds *fds)
{
	getpeername(clt->socket, (struct sockaddr*)&clt->sin, \
			(socklen_t*)&clt->len);
	printf("Client from ip %s , port %d disconnect.\n", \
			inet_ntoa(clt->sin.sin_addr) , ntohs(clt->sin.sin_port));
	FD_CLR(clt->socket, &fds->rd);
	FD_CLR(clt->socket, &fds->wr);
	FD_CLR(clt->socket, &fds->ex);
	ring_zero(clt->ring);
	close(clt->socket);
	clt->socket = 0;
	clt->pos.x = 0;
	clt->pos.y = 0;
}
