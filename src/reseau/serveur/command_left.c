/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_left.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:29:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/18 19:04:07 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_left(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	clt->sens = (clt->sens + 3) % 4;
	send_client_action(clt, true);
	send_graphe_action(srv, clt, 0);
}
