/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:25:40 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 16:23:03 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	command_fork_void(t_fds *fds, char *cmd)
{
	(void)fds;
	(void)cmd;
	return (-1);
}

void		command_fork(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_client	*cur;
	t_client	*new;

	if (!(new = client_init(srv)))
		return (send_client_action(srv, clt, false));
	cur = srv->clt;
	while (cur && cur->next)
		cur = cur->next;
	if ((new->prev = cur))
		cur->next = new;
	else
		srv->clt = new;
	srv->egg++;
	srv->player_max++;
	srv->old_player_max++;
	new->pos.x = clt->pos.x;
	new->pos.y = clt->pos.y;
	new->health = 1260.0f / (float)srv->time;
	new->time = 600.0f / (float)srv->time;
	new->action = command_fork_void(fds, cmd);
	new->team = clt->team;
	new->fork = true;
	new->mom = clt->name;
	send_graphe_action(srv, command_write_msg_end_fork(new), 0, NULL);
	send_client_action(srv, clt, true);
}
