/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_graphe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 16:33:10 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/27 15:34:43 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_gfx[] =
{
	"New graphical client from ip %s, port %d.\nSocket %i is now graphic.\n"
};

static void	command_graphe_log(t_server *srv, t_gfx *gfx, int type)
{
	char	*log;

	if ((!type && asprintf(&log, g_cmd_gfx[0], inet_ntoa(gfx->sin.sin_addr),
					ntohs(gfx->sin.sin_port), gfx->socket)))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
}

void		command_graphe(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	t_client	*clt;
	char		*clt_com;

	if (gfx->isgfx)
		return ;
	gfx->isgfx = true;
	command_graphe_log(srv, gfx, 0);
	clt = srv->clt;
	command_size(fds, srv, gfx, cmd);
	command_map(fds, srv, gfx, cmd);
	command_time_server(fds, srv, gfx, cmd);
	command_team_name(fds, srv, gfx, cmd);
	while (clt)
	{
		if ((clt->health > 0.0f || clt->time > 0.0f) && (clt_com = clt->socket ?
					command_write_msg(clt, 8, 0, srv->team[clt->team]) :
					command_write_msg_end_fork(clt)))
		{
			send(gfx->socket, clt_com, ft_strlen(clt_com), 0);
			ft_memdel((void **)&clt_com);
		}
		clt = clt->next;
	}
}
