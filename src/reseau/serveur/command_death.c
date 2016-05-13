/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_death.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 14:06:53 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:48:30 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	send_client_data(clt);
	send_graphe_action(srv, clt, 0);
	client_zero(clt, fds);
}
