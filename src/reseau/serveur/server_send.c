/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_send.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 18:38:03 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/06 14:11:27 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

typedef void (*ex_gfx)(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd);

static const ex_gfx tfg[] =
{
	player_fork
};

typedef void (*ex_clt)(t_fds *fds, t_server *srv, t_client *clt, char *cmd);

static const ex_clt tfc[] =
{
};

static const char	*gfx_cmd[] =
{
	"client",
	"graphe",
	"contenue"
};

static const char	*clt_cmd[] =
{
	"avance",
	"droite",
	"gauche",
	"voir",
	"inventaire",
	"prendre ",
	"pose ",
	"expulse",
	"broadcast ",
	"incantation",
	"fork",
	"connect_nbr"
};

//	TODO
//	*	clt treat data
bool		send_client(t_fds *fds, t_server *srv, int ret)
{
	int			s;
	int			n;
	char		*cmd;
	t_gfx		*gfx;
	t_client	*clt;

	s = -1;
	cmd = NULL;
	while (ret > 0 && ++s < fds->max && !(n = 0))
	{
		if (!FD_ISSET(s, & fds->wr))
			continue ;
		clt = srv->clt;
		while (clt && clt->socket != s)
			clt = clt->next;
		gfx = srv->gfx;
		while (gfx && gfx->socket != s)
			gfx = gfx->next;
		cmd = (!clt ? ring_send(gfx->ring) : ring_send(clt->ring));
		while (cmd && n < (gfx ? 3 : 12) && strncmp(cmd, (!clt ? gfx_cmd[n] : \
					clt_cmd[n]), strlen(!clt ? gfx_cmd[n] : clt_cmd[n])) != 0)
			n++;
		if (cmd && (!clt ? n < 3 : n < 12))
			!clt ? tfg[n](fds, srv, gfx, cmd) : tfc[n](fds, srv, clt, cmd);
		ft_memdel((void **)&cmd);
	}
	return true;
}
