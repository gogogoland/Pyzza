/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_time_change.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/03 18:50:24 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 21:35:56 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_time_change =
{
	"sgt %d\n"
};

static void command_time_change_player(t_server *srv, int new_time)
{
	t_client	*player;

	player = srv->clt;
	while (player)
	{
		player->time = (player->time * srv->time) / new_time;
		player->health =(player->health * srv->time) / new_time;
		player = player->next;
	}
	srv->time = new_time;
}

void		command_time_change(t_fds *fds,
								t_server *srv,
								t_gfx *gfx,
								char *cmd)
{
	int			new_time;
	char		*box;
	int			i;

	i = 0;
	box = NULL;
	new_time = 0;
	while (cmd[i] > '9' && cmd[i] < '0')
		i++;
	new_time = command_get_int(i - 1, cmd);
	if (new_time > 0 && new_time != srv->time && new_time < TIME_MAX)
		command_time_change_player(srv, new_time);
	asprintf(&box, g_cmd_time_change, srv->time);
	send(gfx->socket, box, strlen(box), 0);
	ft_memdel((void **)&box);
}
