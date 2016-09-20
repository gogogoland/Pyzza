/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_msg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:50:09 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/20 21:52:31 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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

	msg = (char *)malloc((strlen(cmd) - 10) * sizeof(char));
	i = 9;
	while (msg && cmd[i++])
		msg[i - 10] = cmd[i];
	return (msg);
}

static char		*msg_from(int org, char *msg)
{
	char		*tmp;
	char		*pos;

	tmp = ft_itoa(org);
	pos = ft_strjoin(tmp, msg);
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&msg);
	return (pos);
}

void			command_msg(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord		p;
	t_coord		min;
	char		*pos;
	char		*msg;
	t_client	*cur;

	cur = srv->clt;
	msg = msg_contain(cmd);
	while (cur)
	{
		if (cur->socket && cur->socket != clt->socket)
		{
			min = command_msg_min(srv, clt, cur);
			p.x = cur->pos.x - clt->pos.x < 0 ? 0 : 2;
			p.y = cur->pos.y - clt->pos.y < 0 ? 0 : 2;
			p.x = (min.x + (cur->pos.x == clt->pos.x ? 1 : p.x)) % 3;
			p.y = (min.y + (cur->pos.y == clt->pos.y ? 1 : p.y)) % 3;
			pos = msg_from(p.y * 3 + p.x, cmd);
			send(cur->socket, pos, strlen(pos), 0);
			ft_memdel((void **)&pos);
		}
		cur = cur->next;
	}
	send_client_action(clt, true);
	send_graphe_action(srv, command_write_msg(clt, 12, 0, msg), 0, NULL);
}
