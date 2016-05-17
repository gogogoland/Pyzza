/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_nbr_co.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:20:37 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/17 17:25:30 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_nbr_co(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	int			co;
	char		*nc;
	char		*tmp;
	t_client	*cur;

	cur = srv->clt;
	co = 0;
	while (cur)
	{
		co += !cur->socket ? 1 : 0;
		cur = cur->next;
	}
	tmp = ft_itoa(co);
	nc = ft_strjoin(NULL, tmp);
	send(clt->socket, nc, strlen(nc), 0);
	ft_memdel(&tmp);
	ft_memdel(&nc);
}
