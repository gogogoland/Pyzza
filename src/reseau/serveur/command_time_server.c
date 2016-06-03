/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_time_server.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/03 18:45:53 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/03 18:58:44 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		command_time_server(t_fds *fds,
								t_server *srv,
								t_gfx *gfx,
								char *cmd)
{
	char		*box;

	box = NULL;
	asprintf(&box, "sgt %d\n", srv->time);
	send(gfx->socket, box, strlen(box), 0);
	ft_memdel((void **)&box);
}
