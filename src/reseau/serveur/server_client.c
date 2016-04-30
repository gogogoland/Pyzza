/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/29 19:38:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/30 18:28:36 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	client_connect(int s, t_client *clt, t_fds *fds, t_server *srv)
{
	t_client	*new_clt;

	new_clt = clt;
	while (new_clt && new_clt->socket != 0)
		new_clt = new_clt->next;
	if (!new_clt)
		return ;
	new_clt->len = sizeof(struct sockaddr_in);
	if ((s = accept(s, (struct sockaddr *)&new_clt->sin, &new_clt->len)) < 0)
		printf("Error accept %d client", s);
	else
	{
		new_clt->socket = s;
		FD_SET(new_clt->socket, &fds->rd);
		FD_SET(new_clt->socket, &fds->wr);
		FD_SET(new_clt->socket, &fds->ex);
		ft_putendl("New client");
		fds->max = new_clt->socket > fds->max ? new_clt->socket : fds->max;
	}
}

void	client_command(int s, t_client *clt, t_fds *fds, t_server *srv)
{
	t_client	*cur_clt;
	char		buffer[1024];

	cur_clt = clt;
	bzero(buffer, 1024);
	while (cur_clt && cur_clt->socket != s)
		cur_clt = cur_clt->next;
	if (!cur_clt || read(s, buffer, 1024) < 0)
		return ;
	ft_putstr(buffer);
	return ;
}
