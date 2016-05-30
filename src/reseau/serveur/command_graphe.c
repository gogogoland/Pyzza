/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_graphe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 16:33:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/30 18:25:43 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*cmd_new_g =
{
	"New graphical client from ip %s, port %d.\n"
};

void	command_graphe(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	gfx->isgfx = true;
	printf(cmd_new_g, inet_ntoa(gfx->sin.sin_addr) , ntohs(gfx->sin.sin_port));
}
