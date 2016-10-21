/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incant_help_acolyte.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/21 15:25:11 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/21 23:10:47 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_msg_acolyte[] =
{
	"ko\n",
	"elevation en cours, niveau actuel : %i\n",
	"ok\n",
	"pic %i %i #%i",
	"%s #%i",
};

static char	*incant_msg_acolyte_graphic_add(char *s1, char *s2)
{
	char	*ret;

	ret = NULL;
	asprintf(&ret, "%s%s", s1, s2);
	ft_memdel((void **)&s1);
	ft_memdel((void **)&s2);
	return (ret);
}

static void	incant_msg_acolyte_graphic_begin(t_server *srv, t_client *clt,
										int lim_acolyte)
{
	t_client	*acolyte;
	char		*msg;
	char		*tmp;

	msg = NULL;
	tmp = NULL;
	if (asprintf(&msg, g_msg_acolyte[3], clt->pos.x, clt->pos.y, clt->name) < 0)
		return ;
	while (lim_acolyte--)
	{
		acolyte = srv->clt;
		while (acolyte && !(acolyte->name == clt->acolyte[lim_acolyte]
							&& acolyte->socket > 0))
			acolyte = acolyte->next;
		if (acolyte && asprintf(&tmp, g_msg_acolyte[4], msg, acolyte->name) < 0)
		{
			ft_memdel((void **)&msg);
			return ;
		}
		else if (acolyte && msg && tmp)
			msg = incant_msg_acolyte_graphic_add(msg, tmp);
	}
	asprintf(&tmp, "%s\n", msg);
	ft_memdel((void **)&msg);
	send_graphe_action(srv, tmp, 1, clt);
}

static void	incant_msg_acolyte_graphic_end(t_server *srv, t_client *clt,
										int lim_acolyte)
{
	t_client	*acolyte;

	send_graphe_action(srv, command_write_msg_incant_end(clt), 0, NULL);
	send_graphe_action(srv, command_write_msg(clt, 12, clt->lvl, NULL), 0, 0);
	while (lim_acolyte--)
	{
		acolyte = srv->clt;
		while (acolyte && !(acolyte->name == clt->acolyte[lim_acolyte]
							&& acolyte->socket > 0))
			acolyte = acolyte->next;
		if (acolyte)
		{
			send_graphe_action(srv, command_write_msg(acolyte, 12,
							acolyte->lvl, NULL), 0, NULL);
		}
	}
}

void		incant_msg_acolyte(t_server *srv, t_client *clt, int lim_acolyte,
							int state)
{
	t_client	*acolyte;
	int			error;
	char		*msg;

	msg = NULL;
	if ((error = asprintf(&msg, g_msg_acolyte[state + 1], clt->lvl)) > 0)
		send(clt->socket, msg, ft_strlen(msg), 0);
	state ? incant_msg_acolyte_graphic_end(srv, clt, lim_acolyte)
			: incant_msg_acolyte_graphic_begin(srv, clt, lim_acolyte);
	while (error > 0 && lim_acolyte--)
	{
		acolyte = srv->clt;
		while (acolyte && !(acolyte->name == clt->acolyte[lim_acolyte]
							&& acolyte->socket > 0))
			acolyte = acolyte->next;
		if (acolyte)
			send(acolyte->socket, msg, ft_strlen(msg), 0);
		if (state)
		{
			send_graphe_action(srv,
				command_write_msg(clt, 12, clt->lvl, NULL), 0, NULL);
		}
	}
	ft_memdel((void **)&msg);
}
