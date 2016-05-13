/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_forward.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:14:43 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:42:37 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_forward(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord	mv;

	mv.x = (clt->sens - 2) % 2;
	mv.y = (clt->sens - 1) % 2;
	mv.x = (clt->pos.x + mv.x) < 0 ? srv->plateau.x 
		: (clt->pos.x + mv.x)% srv->plateau.x;
	mv.y = (clt->pos.y + mv.y) < 0 ? srv->plateau.y 
		: (clt->pos.y + mv.y) % srv->plateau.y;
	clt->pos.x += mv.x;
	clt->pos.y += mv.y;
	send_graphe_action(srv, clt, 0);
}
