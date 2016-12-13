/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_forward.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:14:43 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/20 20:01:35 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_forward(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord	mv;

	mv.x = (clt->sens - 2) % 2;
	mv.y = (clt->sens - 1) % 2;
	mv.x = (clt->pos.x + mv.x) < 0 ? srv->plateau.x - 1 : mv.x;
	mv.y = (clt->pos.y + mv.y) < 0 ? srv->plateau.y - 1 : mv.y;
	clt->pos.x = (clt->pos.x + mv.x) % srv->plateau.x;
	clt->pos.y = (clt->pos.y + mv.y) % srv->plateau.y;
	send_client_action(clt, true);
	send_graphe_action(srv, command_write_msg(clt, 7, 0, NULL), 0, NULL);
}
