/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_seek.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 20:07:59 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/10 17:52:41 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static char	*command_seek_transfer(char *wt, char *arg, int n)
{
	bool	i;
	char	*ret;

	if (!arg)
		arg = ft_itoa(n);
	ret = ft_strcjoin(wt, arg, ' ');
	ft_memdel((void **)&wt);
	if (i)
		ft_memdel((void **)&arg);
	return (ret);
}

static char	*command_seek_add(t_server *srv, t_coord pos, int n, char *wt)
{
	int			i;
	char		*arg;
	t_client	*clt;

	i = -1;
	if (wt)
		wt = command_seek_transfer(wt, "| ", 0);
	wt = command_seek_transfer(wt, NULL, n);
	while (++i < 7)
	{
		clt = srv->clt;
		wt = command_seek_transfer(wt, NULL, srv->map[pos.y][pos.x][i]);
		while (clt)
		{
			if (clt->socket && clt->pos.x == pos.x && clt->pos.y == pos.y)
			{
				wt = command_seek_transfer(wt, NULL, clt->socket);
				wt = command_seek_transfer(wt, NULL, clt->team);
			}
			clt = clt->next;
		}
	}
	return (wt);
}

static int	command_seek_int(int sens, int pos, t_coord see, int srv)
{
	int	r;

	r = !sens ? pos + see.x - see.y : pos + see.y * sens;
	r = r < 0 ? r - srv : pos;
	r = r >= srv ? r - srv : r;
	return (r);
}

void		command_seek(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord	see;
	t_coord	sens;
	t_coord	pos;
	char	*wt;
	int		n;

	wt = NULL;
	see.y = -1;
	sens.x = (clt->sens - 2) % 2;
	sens.y = (clt->sens - 1) % 2;
	while (++see.y <= clt->lvl)
	{
		see.x = -1;
		while (++see.x < (see.y * 2 + 1))
		{
			pos.x = command_seek_int(sens.x, clt->pos.x, see, srv->plateau.x);
			pos.y = command_seek_int(sens.y, clt->pos.y, see, srv->plateau.y);
		}
	}
	send(clt->socket, wt, strlen(wt), 0);
	send_graphe_action(srv, clt, 0);
	ft_memdel((void **)&wt);
}
