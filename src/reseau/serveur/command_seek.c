/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_seek.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 20:07:59 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/21 20:04:07 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static char	*command_seek_transfer(char *wt, int n, char *arg)
{
	bool	new;
	char	*ret;

	new = false;
	if (!arg)
		arg = ft_itoa(n);
	else
		new = !new;
	ret = ft_strcjoin(wt, arg, ' ');
	ft_memdel((void **)&wt);
	if (!new)
		ft_memdel((void **)&arg);
	return (ret);
}

static char	*command_seek_add(t_server *srv, t_coord pos, int s, char *wt)
{
	int			i;
	char		*arg;
	t_client	*clt;

	i = 0;
	clt = srv->clt;
	if (wt)
		asprintf(&wt, "%s | %i", wt, srv->map[pos.y][pos.x][0]);
	else
		asprintf(&wt, "%i", srv->map[pos.y][pos.x][0]);
	while (++i < 6)
		asprintf(&wt, "%s %i", wt, srv->map[pos.y][pos.x][i]);
	while (clt)
	{
		if (clt->socket && clt->socket != s
			&& clt->pos.x == pos.x && clt->pos.y == pos.y)
			asprintf(&wt, "%i %i", clt->name, clt->team);
		clt = clt->next;
	}
	return (wt);
}

static int	command_seek_int(int sens, int pos, t_coord see, int srv)
{
	int		r;
	bool	deep;
	int		pos_neg;

	deep = (sens % 2) ? false : true;
	pos_neg = (sens == 4 || !sens) ? 1 : -1;
	r = deep ? (see.y * pos_neg) + pos : ((see.x - see.y) * pos_neg) + pos;
	r = r < 0 ? r + srv : r;
	r = r >= srv ? r - srv : r;
	return (r);
}

void		command_seek(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord	see;
	t_coord	sens;
	t_coord	pos;
	char	*wt;

	wt = NULL;
	see.y = -1;
	sens.x = (clt->sens + 1) % 4;
	sens.y = (clt->sens + 2) % 4;
	while (++see.y <= clt->lvl)
	{
		see.x = -1;
		while (++see.x < (see.y * 2 + 1))
		{
			pos.x = command_seek_int(sens.x, clt->pos.x, see, srv->plateau.x);
			pos.y = command_seek_int(sens.y, clt->pos.y, see, srv->plateau.y);
			wt = command_seek_add(srv, pos, clt->socket, wt);
		}
	}
	asprintf(&wt, "%s\n", wt);
	send(clt->socket, wt, strlen(wt), 0);
	ft_memdel((void **)&wt);
}
