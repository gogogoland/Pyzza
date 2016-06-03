/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/02 14:27:13 by tbalea            #+#    #+#             */
/*   Updated: 2016/06/02 16:53:46 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void		command_size(t_fds *fds, t_server *srv, t_gfx *gfx, char *cmd)
{
	char		*box;

	box = NULL;
	asprintf(&box, "msz %i %i\n", srv->plateau.x, srv->plateau.y);
	send(gfx->socket, box, strlen(box), 0);
	ft_memdel((void **)&box);
}
