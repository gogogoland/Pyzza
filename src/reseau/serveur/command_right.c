/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_right.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:27:34 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/21 18:04:35 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char *g_msg_cmd_right = "ppo #%i %i %i %i\n";

static char	*write_msg_cmd_right(t_client *clt)
{
	char	*msg;

	asprintf(&msg, g_msg_cmd_right,
			clt->name,
			clt->pos.x,
			clt->pos.y,
			clt->sens);
	return (msg);
}

void		command_right(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	char	*msg;

	clt->sens = (clt->sens + 1) % 4;
	send_client_action(clt, true);
	msg = write_msg_cmd_right(clt);
	send_graphe_action(srv, command_write_msg(clt, 6, 0, NULL), 0, NULL);
	ft_memdel((void **)&msg);
}
