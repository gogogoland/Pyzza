/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_lvlup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:43:33 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:43:27 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_lvlup(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	if (clt->lvl < 8)
		clt->lvl++;
	send_graphe_action(srv, clt, 0);
}
