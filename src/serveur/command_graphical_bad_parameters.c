/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_graphical_bad_parameters.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/07 22:14:13 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 16:22:09 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_sbp =
{
	"sbp\n"
};

void	command_graphical_bad_parameters(t_fds *fds, t_server *srv, t_gfx *gfx,
										char *cmd)
{
	(void)cmd;
	(void)fds;
	(void)srv;
	send(gfx->socket, g_cmd_sbp, strlen(g_cmd_sbp), 0);
}
