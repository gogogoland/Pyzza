/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_eject.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:01:54 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/22 17:19:50 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_eject_msg = "deplacement %i\n";

static t_coord	assign_coord_move(t_client *clt, t_server *srv)
{
	t_coord	mv;

	if ((mv.x = (2 * ((clt->sens - 2) % 2) + clt->pos.x) % srv->plateau.x) < 0)
		mv.x = srv->plateau.x;
	if ((mv.y = (2 * ((clt->sens - 1) % 2) + clt->pos.y) % srv->plateau.y) < 0)
		mv.y = srv->plateau.y;
	return (mv);
}

static t_coord	assign_coord_targ(t_client *clt, t_server *srv)
{
	t_coord	tg;

	if ((tg.x = ((clt->sens - 2) % 2 + clt->pos.x) % srv->plateau.x) < 0)
		tg.x = srv->plateau.x;
	if ((tg.y = (clt->sens - 1) % 2 + clt->pos.y) % srv->plateau.y < 0)
		tg.y = srv->plateau.y;
	return (tg);
}

static bool		expulse_player(t_server *srv, t_client *cur,
								t_coord mv, char *msg)
{
	if (msg)
		send(cur->socket, msg, ft_strlen(msg), 0);
	cur->pos.x = mv.x;
	cur->pos.y = mv.y;
	cur->action = 0;
	send_graphe_action(srv, command_write_msg(cur, 7, 0, NULL), 0, cur);
	return (true);
}

void			command_eject(t_fds *fds, t_server *srv, t_client *clt, \
								char *cmd)
{
	t_coord		tg;
	t_coord		mv;
	t_client	*cur;
	bool		ok;
	char		*msg;

	ok = false;
	msg = NULL;
	cur = srv->clt;
	mv = assign_coord_move(clt, srv);
	tg = assign_coord_targ(clt, srv);
	send_graphe_action(srv, command_write_msg(clt, 5, 0, NULL), 0, NULL);
	if (asprintf(&msg, g_cmd_eject_msg, ((clt->sens + 2) % 4)) < 1)
		msg = NULL;
	while (cur)
	{
		if (cur && cur->socket && cur->pos.x == tg.x && cur->pos.y == tg.y)
			ok = expulse_player(srv, cur, mv, msg);
		cur = cur->next;
	}
	if (msg)
		ft_memdel((void **)&msg);
	send_client_action(srv, clt, ok);
}
