/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_incant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:40:06 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 21:53:29 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_incant =
{
	"elevation en cours niveau actuel : %i\n"
};

//TODO
//	consomme ressource
void	command_incant(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	char	*msg;

	msg = NULL;
	if (++clt->lvl > 8)
		clt->lvl = 8;
	if (asprintf(&msg, g_cmd_incant, clt->lvl) > 0)
	{
		send(clt->socket, msg, strlen(msg), 0);
		ft_memdel((void **)&msg);
	}
}
