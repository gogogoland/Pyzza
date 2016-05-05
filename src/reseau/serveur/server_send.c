/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_send.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 18:38:03 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/05 19:45:05 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"server.h"

static void	exec_command(t_client *clt, t_gfx *gfx, t_server *srv, char *cmd)
{

	if (!cmd)
		return ;
	printf("%s\n", cmd);
}

//	TODO
//	*	clt treat data
//	*	transfer data from gfx to clt
bool		send_client(t_fds *fds, t_server *srv, int ret)
{
	int			s;
	char		*cmd;
	t_gfx		*gfx;
	t_client	*clt;

	s = -1;
	cmd = NULL;
	if (ret <= 0)
		return true;
	while (++s < fds->max)
	{
		if (!FD_ISSET(s, & fds->wr))
			continue ;
		clt = srv->clt;
		while (clt && clt->socket != s && clt->next)
			clt = clt->next;
		gfx = srv->gfx;
		while (gfx && gfx->socket != s && gfx->next)
			gfx = gfx->next;
		if (gfx)
			cmd = ring_send(gfx->ring);
//		if (clt)
//			cmd = ring_send(clt->ring);
		exec_command(clt, gfx, srv, cmd);
	}
	return true;
}
