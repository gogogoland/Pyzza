/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_nbr_co.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:20:37 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 21:19:23 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_nbr_co(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int			co;
	char		*msg;
	t_client	*cur;

	cur = srv->clt;
	msg = NULL;
	co = 0;
	while (cur)
	{
		co += (!cur->socket && (!cur->team || cur->team == clt->team)) ? 1 : 0;
		cur = cur->next;
	}
	if (asprintf(&msg, "%i\n", co) > 0)
	{
		send(clt->socket, msg, strlen(msg), 0);
		ft_memdel((void **)&msg);
	}
}
