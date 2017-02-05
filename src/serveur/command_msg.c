/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_msg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:50:09 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/05 18:51:48 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_msg_cmd_msg = "message %i, %s\n";

static t_coord	command_msg_org_pos(t_server *srv, t_client *clt, t_client *cur)
{
	t_coord	tc;

	tc.x = (cur->pos.x - clt->pos.x) * (2 * (cur->pos.x > clt->pos.x) - 1);
	tc.y = (cur->pos.y - clt->pos.y) * (2 * (cur->pos.y > clt->pos.y) - 1);
	tc.x = 1 - (2 * (tc.x > (srv->plateau.x - tc.x)));
	tc.y = 1 - (2 * (tc.y > (srv->plateau.y - tc.y)));
	tc.x = ((cur->pos.x < clt->pos.x) - (cur->pos.x > clt->pos.x)) * tc.x;
	tc.y = ((cur->pos.y < clt->pos.y) - (cur->pos.y > clt->pos.y)) * tc.y;
	return (tc);
}

static int		command_msg_org_dir(t_coord tc, t_client *cur)
{
	int		origin;

	if (!tc.x && !tc.y)
		return (0);
	else if (tc.y == -1 && tc.x == 0)
		origin = 0;
	else if (tc.y == -1 && tc.x == -1)
		origin = 1;
	else if (tc.y == 0 && tc.x == -1)
		origin = 2;
	else if (tc.y == 1 && tc.x == -1)
		origin = 3;
	else if (tc.y == 1 && tc.x == 0)
		origin = 4;
	else if (tc.y == 1 && tc.x == 1)
		origin = 5;
	else if (tc.y == -1 && tc.x == 1)
		origin = 7;
	else if (tc.y == 0 && tc.x == 1)
		origin = 6;
	origin = (origin + cur->sens * 2 + cur->sens % 2 * 4) % 8 + 1;
	return (origin);
}

static void		msg_send_player(int org, char *msg, int socket)
{
	char		*pos;

	if (asprintf(&pos, g_msg_cmd_msg, org, msg))
	{
		send(socket, pos, strlen(pos), 0);
		ft_memdel((void **)&pos);
	}
}

void			command_msg(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int			org;
	t_coord		min;
	t_client	*cur;

	cur = srv->clt;
	while (clt->current_cmd && cur)
	{
		if (cur->socket > 0 && cur->socket != clt->socket)
		{
			org = command_msg_org_dir(command_msg_org_pos(srv, clt, cur), cur);
			msg_send_player(org, clt->current_cmd, cur->socket);
		}
		cur = cur->next;
	}
	send_client_action(srv, clt, !!clt->current_cmd);
	if (clt->current_cmd)
	{
		send_graphe_action(srv, command_write_msg(clt, 14, 0, clt->current_cmd),
							0, NULL);
	}
	erasecur_cmd(clt);
	oldtonew_cmd(clt);
}
