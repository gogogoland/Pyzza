/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_eject.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:01:54 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/18 18:54:13 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static t_coord	assign_coord(int x, int y)
{
	t_coord	ret;

	ret.x = x;
	ret.y = y;
	return (ret);
}

void			command_eject(t_fds *fds, t_server *srv, t_client *clt, \
								char *cmd)
{
	t_coord		cp;
	t_coord		mv;
	t_client	*cur;
	bool		ok;
	char		*tmp;

	ok = false;
	cur = srv->clt;
	mv = assign_coord((clt->sens - 2) % 2, (clt->sens - 1) % 2);
	cp = assign_coord(clt->pos.x + mv.x, clt->pos.y + mv.y);
	mv.x = (cp.x + mv.x) < 0 ? srv->plateau.x : (cp.x + mv.x) % srv->plateau.x;
	mv.y = (cp.y + mv.y) < 0 ? srv->plateau.y : (cp.y + mv.y) % srv->plateau.y;
	while (cur)
	{
		if (cur && cur->socket && cur->pos.x == cp.x && cur->pos.y == cp.y)
		{
			cur->pos = assign_coord(mv.x, mv.y);
			cur->action = 0;
			send_graphe_action(srv, cur, 0);
			ok = true;
		}
		cur = cur->next;
	}
	send_client_action(clt, ok);
	send_graphe_action(srv, clt, 0);
}
