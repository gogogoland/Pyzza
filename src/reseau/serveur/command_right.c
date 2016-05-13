/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_right.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:27:34 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:46:29 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_right(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	clt->sens = (clt->sens + 1) % 4;
	send_graphe_action(srv, clt, 0);
}
