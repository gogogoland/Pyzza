/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_death.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 14:06:53 by tbalea            #+#    #+#             */
/*   Updated: 2017/01/07 04:54:54 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_death[] =
{
	"Player #%i from ip %s, port %d has disconnected.\n",
	"Socket %i closed.\n"
};

static void	command_death_log(t_server *srv, int type, char *ip, int port)
{
	char	*log;

	if (type ? asprintf(&log, g_cmd_death[!type], type, ip, port) :
				asprintf(&log, g_cmd_death[!type], port))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
}

static void	command_death_fork(t_fds *fds, t_server *srv, t_client *clt)
{
	t_client	*prev;

	prev = srv->clt;
	while (prev && prev->next != clt)
		prev = prev->next;
	prev->next = clt->next;
	if (clt->next)
		clt->next->prev = prev;
	client_kill(clt, fds);
	srv->player_max--;
	srv->old_player_max--;
}

void		command_death(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	if (clt->socket)
	{
		getpeername(clt->socket, (struct sockaddr*)&clt->sin,
					(socklen_t*)&clt->len);
		command_death_log(srv, clt->name, inet_ntoa(clt->sin.sin_addr),
				ntohs(clt->sin.sin_port));
		command_death_log(srv, 0, NULL, clt->socket);
		send(clt->socket, "mort\n", 4, 0);
		send_graphe_action(srv, command_write_msg(clt, 4, 0, NULL), 0, NULL);
	}
	else
		send_graphe_action(srv, command_write_msg(clt, 3, 0, NULL), 0, NULL);
	clt->fork ? command_death_fork(fds, srv, clt) : client_zero(clt, fds, srv);
}
