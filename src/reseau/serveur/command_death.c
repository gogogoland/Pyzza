/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_death.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 14:06:53 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/20 21:59:09 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		command_death(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	if (clt->socket)
	{
		send(clt->socket, "mort\n", 5, 0);
		send_graphe_action(srv, command_write_msg(clt, 5, 0, NULL), 0, NULL);
	}
	else
		send_graphe_action(srv, command_write_msg(clt, 4, 0, NULL), 0, NULL);
	client_zero(clt, fds);
}
