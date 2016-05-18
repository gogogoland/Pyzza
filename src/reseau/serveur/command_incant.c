/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_incant.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/17 17:40:06 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/18 19:15:22 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*cmd_lvling =
{
	"elevation en cours niveau actuel : "
};

//TODO
//	consomme ressource
void	command_incant(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	char	*str;
	char	*lvl;

	if (++clt->lvl > 8)
		clt->lvl = 8;
	lvl = ft_itoa(clt->lvl);
	str = ft_strjoin(cmd_lvling, lvl);
	send(clt->socket, str, strlen(str), 0);
	ft_memdel((void **)&lvl);
	ft_memdel((void **)&str);
	return ;	
}
