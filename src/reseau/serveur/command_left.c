/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_left.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:29:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:43:20 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_left(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	clt->sens = (clt->sens + 3) % 4;
	send_graphe_action(srv, clt, 0);
}
