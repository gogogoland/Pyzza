/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_graphe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 15:04:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/01 23:59:08 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_sgm[] =
{
	"Bienvenue\n",
	"Malloc failed for new client.\n",
	"Accept failed new connection.\n",
	"New undefined client from ip %s , port %d.\nSocket %i has opened.\n",
	"Graphical client from ip %s , port %d has disconnected.\n\
	Socket %i has closed.\n",
	"Undefined client from ip %s , port %d has disconnected.\n\
	Socket %i has closed.\n",
	"Close for graphical client from ip %s , port %d failed.\n\
	Socket %i is closed.\n",
	"Close for undefined client from ip %s , port %d failed.\n\
	Socket %i is closed.\n"
};

static void	graphe_log(t_server *srv, int type, t_gfx *gfx)
{
	char	*log;

	if ((type < 3 && asprintf(&log, "%s", g_sgm[type]))
			|| (type > 2 && asprintf(&log, g_sgm[type],
										inet_ntoa(gfx->sin.sin_addr),
										ntohs(gfx->sin.sin_port),
										gfx->socket)))
	{
		server_log(srv, log);
		ft_memdel((void **)&log);
	}
}

t_gfx		*graphe_news(t_server *srv, t_gfx *prev, t_fds *fds, int s)
{
	t_gfx	tmp;
	t_gfx	*gfx;

	tmp.len = sizeof(struct sockaddr_in);
	if (!(gfx = (t_gfx *)malloc(sizeof(t_gfx))))
	{
		if ((s = accept(s, (struct sockaddr *)&tmp.sin, &tmp.len)) < 0)
			close(s);
		graphe_log(srv, 1, gfx);
		return (gfx);
	}
	gfx->len = sizeof(struct sockaddr_in);
	if ((gfx->socket = accept(s, (struct sockaddr *)&gfx->sin, &gfx->len)) < 0)
	{
		graphe_log(srv, 2, gfx);
		graphe_kill(srv, gfx, fds, false);
		return (NULL);
	}
	graphe_log(srv, 3, gfx);
	gfx->ring = ring_init(srv, 1);
	gfx->isgfx = false;
	gfx->prev = prev;
	gfx->next = NULL;
	send(gfx->socket, g_sgm[0], strlen(g_sgm[0]), 0);
	return (gfx);
}

t_gfx		*graphe_init(t_server *srv)
{
	t_gfx	*gfx;

	if (!(gfx = (t_gfx *)malloc(sizeof(t_gfx))))
	{
		graphe_log(srv, 1, gfx);
		return (gfx);
	}
	gfx->socket = 0;
	gfx->ring = ring_init(srv, 1);
	gfx->prev = NULL;
	gfx->next = NULL;
	return (gfx);
}

//	TODO
//	*	set to NULL gfx->sin ?
void		graphe_kill(t_server *srv, t_gfx *gfx, t_fds *fds, bool gfxtoclt)
{
	t_gfx	*next;

	next = gfx->next;
	FD_CLR(gfx->socket, &fds->rd);
	FD_CLR(gfx->socket, &fds->wr);
	FD_CLR(gfx->socket, &fds->ex);
	ring_kill(gfx->ring);
	if (!gfxtoclt)
	{
		graphe_log(srv, ((gfx->isgfx ? 4 : 5) + (!close(gfx->socket) ? 0 : 2)),
					gfx);
	}
	gfx->isgfx = false;
	gfx->len = 0;
	//*(gfx)->sin = NULL;
	gfx->socket = 0;
	if (gfx->prev)
		gfx->prev->next = gfx->next;
	if (gfx->next)
		gfx->next->prev = gfx->prev;
	gfx->prev = NULL;
	gfx->next = NULL;
	free(gfx);
	gfx = next;
}
