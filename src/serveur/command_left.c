/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_left.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:29:04 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 16:07:41 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_left(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	(void)fds;
	(void)cmd;
	clt->sens = (clt->sens + 1) % 4;
	send_client_action(srv, clt, true);
	send_graphe_action(srv, command_write_msg(clt, 7, 0, NULL), 0, NULL);
}
