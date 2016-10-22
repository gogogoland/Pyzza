/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_msg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:50:09 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/22 16:39:57 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_msg_cmd_msg = "message %i, %s";

static t_coord	command_msg_min(t_server *srv, t_client *clt, t_client *cur)
{
	t_coord	d1;
	t_coord	d2;
	t_coord	re;

	d1.x = cur->pos.x - clt->pos.x < 0 ? clt->pos.x - cur->pos.x
		: cur->pos.x - clt->pos.x;
	d1.y = cur->pos.y - clt->pos.y < 0 ? clt->pos.y - cur->pos.y
		: cur->pos.y - clt->pos.y;
	d2.x = cur->pos.x < clt->pos.x ? srv->plateau.x - cur->pos.x + clt->pos.x
		: srv->plateau.x - clt->pos.x + cur->pos.x;
	d2.y = cur->pos.y < clt->pos.y ? srv->plateau.y - cur->pos.y + clt->pos.y
		: srv->plateau.y - clt->pos.y + cur->pos.y;
	re.x = d1.x < d2.x ? 0 : 2;
	re.y = d2.y < d2.y ? 0 : 2;
	return (re);
}

static char		*msg_contain(char *cmd)
{
	char	*msg;
	int		i;

	if (!cmd || strlen(cmd) < 10)
		return (NULL);
	msg = (char *)malloc((strlen(cmd) - 9) * sizeof(char));
	i = 8;
	while (msg && cmd[i++])
		msg[i - 9] = cmd[i];
	return (msg);
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

bool			msg_save(t_client *clt, char *msg)
{
	if (!clt || !msg)
		return (false);
	if (clt->current_cmd)
		ft_memdel((void **)&clt->current_cmd);
	clt->current_cmd = msg_contain(msg);
	return (true);
}

void			command_msg(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord		p;
	t_coord		min;
	t_client	*cur;

	cur = srv->clt;
	while (clt->current_cmd && cur)
	{
		if (cur->socket && cur->socket != clt->socket)
		{
			min = command_msg_min(srv, clt, cur);
			p.x = cur->pos.x - clt->pos.x < 0 ? 0 : 2;
			p.y = cur->pos.y - clt->pos.y < 0 ? 0 : 2;
			p.x = (min.x + (cur->pos.x == clt->pos.x ? 1 : p.x)) % 3;
			p.y = (min.y + (cur->pos.y == clt->pos.y ? 1 : p.y)) % 3;
			msg_send_player(p.y * 3 + p.x, clt->current_cmd, cur->socket);
		}
		cur = cur->next;
	}
	send_client_action(clt, !!clt->current_cmd);
	send_graphe_action(srv, command_write_msg(clt, 14, 0, clt->current_cmd),
						0, NULL);
	if (clt && clt->current_cmd)
		ft_memdel((void **)&clt->current_cmd);
}
