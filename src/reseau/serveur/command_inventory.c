/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_inventory.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:32:11 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/13 14:43:00 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_inventory(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int		i;
	char	*tmp;
	char	*rsc;

	i = -1;
	while (++i < 7)
	{
		tmp = ft_itoa(clt->rsc[i]);
		ft_strjoin(rsc, tmp);
		ft_memdel((void **)&tmp);
	}
	send(clt->socket, rsc, strlen(rsc), 0);
	send_graphe_action(srv, clt, 0);
	ft_memdel((void **)&rsc);
}
