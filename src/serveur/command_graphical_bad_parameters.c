/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_graphical_bad_parameters.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/07 22:14:13 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/07 22:16:40 by tbalea           ###   ########.fr       */
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
	send(gfx->socket, g_cmd_sbp, strlen(g_cmd_sbp), 0);
}
