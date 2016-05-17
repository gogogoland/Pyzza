/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_graphe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 15:04:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/17 14:49:11 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*sgm[] =
{
	"Malloc failed for new client.\n",
	"Accept failed new connection.\n",
	"New undefined client from ip %s , port %d.\n",
	"Graphical client from ip %s , port %d has disconnected.\n",
	"Undefined client from ip %s , port %d has disconnected.\n"
};

t_gfx	*graphe_news(t_gfx *prev, t_fds *fds, int s)
{
	t_gfx	tmp;
	t_gfx	*gfx;

	tmp.len = sizeof(struct sockaddr_in);
	if (!(gfx = (t_gfx *)malloc(sizeof(t_gfx))) && printf("%s", sgm[0]))
	{
		if ((s = accept(s, (struct sockaddr *)&tmp.sin, &tmp.len)) >= 0)
			close(s);
		return (gfx);
	}
	gfx->len = sizeof(struct sockaddr_in);
	if ((gfx->socket = accept(s, (struct sockaddr *)&gfx->sin, &gfx->len)) < 0)
	{
		printf("%s", sgm[1]);
		graphe_kill(gfx, fds, false);
		return (NULL);
	}
	printf(sgm[2], inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port));
	gfx->ring = ring_init(1);
	gfx->isgfx = false;
	gfx->prev = prev;
	gfx->next = NULL;
	return (gfx);
}

t_gfx	*graphe_init(void)
{
	t_gfx	*gfx;

	if (!(gfx = (t_gfx *)malloc(sizeof(t_gfx))) && printf("%s", sgm[0]))
		return (gfx);
	gfx->socket = 0;
	gfx->ring = ring_init(1);
	gfx->prev = NULL;
	gfx->next = NULL;
	return (gfx);
}

//	TODO
//	*	set to NULL gfx->sin
void		graphe_kill(t_gfx *gfx, t_fds *fds, bool gfxtoclt)
{
	t_gfx	*next;

	next = gfx->next;
	FD_CLR(gfx->socket, &fds->rd);
	FD_CLR(gfx->socket, &fds->wr);
	FD_CLR(gfx->socket, &fds->ex);
	ring_kill(gfx->ring);
	if (!gfxtoclt)
	{
		close(gfx->socket);
		printf(gfx->isgfx ? sgm[3] : sgm[4], \
				inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port));
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
