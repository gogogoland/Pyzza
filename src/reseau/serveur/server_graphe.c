/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_graphe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 15:04:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/04 16:36:15 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

t_gfx	*graphe_init(t_gfx *prev, t_tmp tmp)
{
	t_gfx	*gfx;

	if (!(gfx = (t_gfx *)malloc(sizeof(t_gfx))))
		return (gfx);
	gfx->socket = tmp.socket;
	gfx->len = tmp.len;
	gfx->sin = tmp.sin;
	gfx->prev = prev;
	return (gfx);
}

void		graphe_kill(t_gfx *gfx, t_fds *fds)
{
	FD_CLR(gfx->socket, &fds->rd);
	FD_CLR(gfx->socket, &fds->wr);
	FD_CLR(gfx->socket, &fds->ex);
	close(gfx->socket);
	gfx->socket = 0;
	if (gfx->prev)
		gfx->prev->next = gfx->next;
	if (gfx->next)
		gfx->next->prev = gfx->prev;
	free(gfx);
	gfx = NULL;
}
