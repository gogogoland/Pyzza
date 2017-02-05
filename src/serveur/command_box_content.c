/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_box_content.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/07 14:47:39 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/01 13:47:22 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_cmd_box_content =
{
	"bct %i %i %i %i %i %i %i %i %i %i\n"
};

void		command_box_content(t_gfx *gfx, int x, int y, int *box)
{
	char	*content;

	content = NULL;
	asprintf(&content, g_cmd_box_content,
		x, y, box[0], box[1], box[2], box[3], box[4], box[5], box[6], box[7]);
	if (content)
	{
		if (gfx)
			send(gfx->socket, content, strlen(content), 0);
		ft_memdel((void **)&content);
	}
}
