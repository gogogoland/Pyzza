/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_box_content.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/21 20:11:25 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	command_box_content(t_gfx *gfx, int x, int y, int *box)
{
	char	*content;

	content = NULL;
	asprintf(&content, "bct %i %i %i %i %i %i %i %i %i %i\n",
		x, y, box[0], box[1], box[2], box[3], box[4], box[5], box[6], box[7]);
	if (content)
	{
		send(gfx->socket, content, strlen(content), 0);
		server_log(content);
	}
	ft_memdel((void **)&content);
}
