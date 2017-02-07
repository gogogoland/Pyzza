/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_seek.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 20:07:59 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 16:18:23 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_seek[] =
{
	"nourriture",
	"linemate",
	"deraumere",
	"sibur",
	"mendiane",
	"phiras",
	"thystame",
	"joueur"
};

static char	*command_seek_copy(char *wt, int type)
{
	char	*new;

	new = NULL;
	if (type == -1 && !wt)
		asprintf(&new, "");
	else if (type == -1 && wt)
	{
		asprintf(&new, "%s |", wt);
		ft_memdel((void **)&wt);
	}
	else if (type < 8 && type > -1 && wt)
	{
		asprintf(&new, "%s %s", wt, g_cmd_seek[type]);
		ft_memdel((void **)&wt);
	}
	else if (type < 8 && type > -1 && !wt)
		asprintf(&new, "%s", g_cmd_seek[type]);
	return (new);
}

static char	*command_seek_add(t_server *srv, t_coord pos, int s, char *wt)
{
	int			i;
	int			n;
	t_client	*clt;

	i = -1;
	clt = srv->clt;
	wt = command_seek_copy(wt, -1);
	while (++i < 7)
	{
		n = srv->map[pos.y][pos.x][i];
		while (n-- > 0)
			wt = command_seek_copy(wt, i);
	}
	while (clt)
	{
		if (clt->socket && clt->socket != s
				&& clt->pos.x == pos.x && clt->pos.y == pos.y)
			wt = command_seek_copy(wt, 7);
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
	pos_neg = (sens == 3 || !sens) ? 1 : -1;
	r = deep ? (see.y * pos_neg) + pos : ((see.x - see.y) * pos_neg) + pos;
	r = r < 0 ? r + srv : r;
	r = r >= srv ? r - srv : r;
	return (r);
}

static int	command_seek_void(char *cmd, t_fds *fds)
{
	(void)fds;
	(void)cmd;
	return (-1);
}

void		command_seek(t_fds *fds, t_server *srv, t_client *clt, char *cmd)
{
	t_coord	see;
	t_coord	sens;
	t_coord	pos;
	char	*wt;
	char	*tmp;

	wt = NULL;
	tmp = NULL;
	see.y = command_seek_void(cmd, fds);
	sens.x = (clt->sens + 1) % 4;
	sens.y = (clt->sens + 2) % 4;
	while (++see.y <= clt->lvl)
	{
		see.x = (see.y * 2 + 1);
		while (--see.x >= 0)
		{
			pos.x = command_seek_int(sens.x, clt->pos.x, see, srv->plateau.x);
			pos.y = command_seek_int(sens.y, clt->pos.y, see, srv->plateau.y);
			tmp = command_seek_add(srv, pos, clt->socket, tmp);
		}
	}
	asprintf(&wt, "%s\n", tmp);
	send(clt->socket, wt, strlen(wt), 0);
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&wt);
}
