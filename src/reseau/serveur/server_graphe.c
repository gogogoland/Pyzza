/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_graphe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/04 15:04:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/12 19:03:40 by tbalea           ###   ########.fr       */
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

t_gfx	*graphe_init(t_gfx *prev, t_fds *fds, int s)
{
	t_gfx	tmp;
	t_gfx	*gfx;

ft_putendl("server_graphe	init	0");
	tmp.len = sizeof(struct sockaddr_in);
	if (!(gfx = (t_gfx *)malloc(sizeof(t_gfx))) && printf("%s", sgm[0]))
	{
ft_putendl("server_graphe	init	1");
		if ((s = accept(s, (struct sockaddr *)&tmp.sin, &tmp.len)) >= 0)
			close(s);
ft_putendl("server_graphe	init	2");
		return (gfx);
	}
ft_putendl("server_graphe	init	3");
	gfx->len = sizeof(struct sockaddr_in);
	if ((gfx->socket = accept(s, (struct sockaddr *)&gfx->sin, &gfx->len)) < 0)
	{
ft_putendl("server_graphe	init	4");
		printf("%s", sgm[1]);
		graphe_kill(gfx, fds, false);
ft_putendl("server_graphe	init	5");
		return (NULL);
	}
ft_putendl("server_graphe	init	6");
	printf(sgm[2], inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port));
	gfx->ring = ring_init(1);
	gfx->isgfx = false;
	gfx->prev = prev;
	gfx->next = NULL;
ft_putendl("server_graphe	init	7");
	return (gfx);
}

//	TODO
//	*	set to NULL gfx->sin
void		graphe_kill(t_gfx *gfx, t_fds *fds, bool gfxtoclt)
{
	t_gfx	*next;

t_gfx	*tmp;
int		i;

i = 0;
tmp	= gfx;
while (tmp)
{
ft_putendl("");
	printf("socket = %d, i = %d\n", tmp->socket, i);
	i++;
	tmp = tmp->next;
ft_putendl("");
}

	next = gfx->next;
ft_putendl("server_graphe	kill	0");
	FD_CLR(gfx->socket, &fds->rd);
	FD_CLR(gfx->socket, &fds->wr);
	FD_CLR(gfx->socket, &fds->ex);
ft_putendl("server_graphe	kill	1");
	ring_kill(gfx->ring);
ft_putendl("server_graphe	kill	2");
	if (!gfxtoclt)
	{
ft_putendl("server_graphe	kill	3");
		close(gfx->socket);
		printf(gfx->isgfx ? sgm[3] : sgm[4], \
				inet_ntoa(gfx->sin.sin_addr), ntohs(gfx->sin.sin_port));
	}
ft_putendl("server_graphe	kill	4");
	gfx->isgfx = false;
	gfx->len = 1;
	//*(gfx)->sin = NULL;
	gfx->socket = 0;
	if (gfx->prev)
		gfx->prev->next = gfx->next;
ft_putendl("server_graphe	kill	5");
	if (gfx->next)
		gfx->next->prev = gfx->prev;
ft_putendl("server_graphe	kill	6");
	//gfx->prev = NULL;
	//gfx->next = NULL;
	free(gfx);
	gfx = next;
ft_putendl("server_graphe	kill	7");
}
