/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_eat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 16:00:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/28 18:57:40 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_eat(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int		i;
	int		rsc;
	bool	ook;
	char	**tab;

	ook = false;
	if (clt->pos.rsc[0] > 0)
	{
		clt->pos.rsc[0]--;
		clt->health += (126 / srv->time);
		ook = true;
		send_graphe_action(srv, command_write_msg(clt, 6, 0, NULL), 0, NULL);
		send_graphe_action(srv, command_write_msg(clt, 9, 0, NULL), 1, clt);
	}
	send_client_action(clt, ook);
}
