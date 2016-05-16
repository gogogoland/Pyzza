/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_send.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 18:38:03 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/16 14:51:37 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

typedef void (*ex_gfx)(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd);

static const ex_gfx tfg[] =
{
	player_fork,
	command_graphe,
	command_map
};

typedef void (*ex_clt)(t_fds *fds, t_server *srv, t_client *clt, char *cmd);

static const ex_clt tfc[] =
{
	command_forward,
	command_right,
	command_left,
	command_seek,
	command_inventory,
	command_take,
	command_pose,
	command_eject,
	command_msg,
	command_msg,
	command_msg,
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
	"prend",
	"pose",
	"expulse",
	"broadcast",
	"incantation",
	"fork",
	"connect_nbr"
};

//	TODO
//	*	clt treat data
bool	send_client(t_fds *fds, t_server *srv, int ret)
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
		while (gfx != NULL && gfx->socket != s)
{
			gfx = gfx->next;
}
		if ((clt && clt->ring)|| (!clt && gfx && gfx->ring))
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

void	send_client_data(t_client *clt)
{
	send(clt->socket, "Ok\n", 3, 0);
}

void	send_graphe_action(t_server *srv, t_client *clt, int n)
{
	t_gfx	*gfx;
	char	*action;
	char	*tmp1;
	char	*tmp2;

	gfx = srv->gfx;
	tmp1 = ft_itoa(clt->socket);
	tmp2 = ft_strjoin("action ", tmp1);
	ft_memdel((void **)&tmp1);
	action = ft_strcjoin(tmp2, clt_cmd[n], ' ');
	ft_memdel((void **)&tmp2);
	while (gfx)
	{
		send(gfx->socket, action, strlen(action), 0);
		gfx = gfx->next;
	}
	ft_memdel((void **)&action);
}
