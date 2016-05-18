/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_death.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 14:06:53 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/18 18:56:19 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_death(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	send(clt->socket, "mort\n", 5, 0);
	send_graphe_action(srv, clt, 0);
	client_zero(clt, fds);
}
