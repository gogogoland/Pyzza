/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_fork.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:25:40 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/18 19:14:58 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_fork(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_client	*cur;
	t_client	*new;

	srv->player_max = srv->old_player_max + 1;
	srv->old_player_max = srv->player_max;
	new = srv->clt;
	while (new)
		new = new->next;
	if (!(new = client_init()))
		return ;
	new->pos.x = clt->pos.x;
	new->pos.y = clt->pos.y;
	new->health = 642.0f / (float)srv->time;
	new->team = clt->team;
	new->fork = true;
	send_client_action(clt, true);
}
