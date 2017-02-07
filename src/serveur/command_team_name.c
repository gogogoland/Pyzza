/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_team_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/10 16:33:10 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 16:21:40 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_team_name =
{
	"tna %s\n"
};

void	command_team_name(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	char		*team;
	int			i;

	(void)fds;
	(void)cmd;
	i = -1;
	team = NULL;
	while (srv && srv->team && srv->team[++i])
	{
		if (asprintf(&team, g_cmd_team_name, srv->team[i]))
		{
			send(gfx->socket, team, ft_strlen(team), 0);
			ft_memdel((void **)&team);
		}
	}
}
