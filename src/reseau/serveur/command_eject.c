/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_eject.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:01:54 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:52:28 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		command_eject(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord		cp;
	t_coord		mv;
	t_client	*cur;
	char		*tmp;

	cur = srv->clt;
	mv.x = (clt->sens - 2) % 2;
	mv.y = (clt->sens - 1) % 2;
	cp.x = clt->pos.x + mv.x;
	cp.y = clt->pos.y + mv.y;
	mv.x = (cp.x + mv.x) < 0 ? srv->plateau.x : (cp.x + mv.x) % srv->plateau.x;
	mv.y = (cp.y + mv.y) < 0 ? srv->plateau.y : (cp.y + mv.y) % srv->plateau.y;
	while (cur)
	{
		if (cur && cur->socket && cur->pos.x == cp.x && cur->pos.y == cp.y)
		{
			cur->pos.x = mv.x;
			cur->pos.y = mv.y;
			send_graphe_action(srv, cur, 0);
		}
		cur = cur->next;
	}
	send_graphe_action(srv, clt, 0);
}
