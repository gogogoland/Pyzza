/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_eat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 16:00:39 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/08 18:46:52 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_eat(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	bool	ook;

	(void)cmd;
	(void)fds;
	ook = false;
	if (clt->pos.rsc[0] > 0)
	{
		clt->pos.rsc[0]--;
		if ((clt->health += (126000000 / srv->time)) < -9000000000000000000)
			clt->health = 9223372036854775807;
		ook = true;
		send_graphe_action(srv, command_write_msg(clt, 6, 0, NULL), 0, NULL);
		send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 1, clt);
	}
	send_client_action(srv, clt, ook);
}
