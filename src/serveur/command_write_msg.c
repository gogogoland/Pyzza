/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_write_msg.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/20 17:18:51 by tbalea            #+#    #+#             */
/*   Updated: 2017/01/06 19:52:11 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char *g_msg_cmd[] =
{
	"ebo #%i\n",
	"pfk #%i\n",
	"eht #%i\n",
	"edi #%i\n",
	"pdi #%i\n",
	"pex #%i\n",
	"pet #%i\n",
	"ppo #%i %i %i %i\n",
	"pnw #%i %i %i %i %i %s\n",
	"pin #%i %i %i %i %i %i %i %i %i %i\n",
	"pgt #%i %i\n",
	"pdr #%i %i\n",
	"plv #%i %i\n",
	"seg #%i %s\n",
	"pbc #%i %s\n",
	"pie %i %i %i\n",
	"enw #%i #%i %i %i\n",
};

char	*command_write_msg(t_client *clt, int action, int n, char *pbc)
{
	char	*msg;
	int		*rsc;

	if (action < 7 && asprintf(&msg, g_msg_cmd[action], clt->name) >= 0)
		return (msg);
	else if (action == 7 && asprintf(&msg, g_msg_cmd[action], clt->name,
				clt->pos.x, clt->pos.y, clt->sens) >= 0)
		return (msg);
	else if (action == 8 && asprintf(&msg, g_msg_cmd[action], clt->name,
				clt->pos.x, clt->pos.y, clt->sens, clt->lvl, pbc) >= 0)
		return (msg);
	else if (action == 9)
	{
		rsc = clt->pos.rsc;
		if (asprintf(&msg, g_msg_cmd[action], clt->name, clt->pos.x, clt->pos.y,
				rsc[0], rsc[1], rsc[2], rsc[3], rsc[4], rsc[5], rsc[6]) >= 0)
			return (msg);
	}
	else if (action < 13
			&& asprintf(&msg, g_msg_cmd[action], clt->name, n) >= 0)
		return (msg);
	else if (action < 15
			&& asprintf(&msg, g_msg_cmd[action], clt->name, pbc) >= 0)
		return (msg);
	return (NULL);
}

char	*command_write_msg_incant_end(t_client *clt)
{
	char *msg;

	asprintf(&msg, g_msg_cmd[15],
				clt->pos.x, clt->pos.y, clt->lvl == clt->tolvl);
	return (msg);
}

char	*command_write_msg_end_fork(t_client *frk)
{
	char *msg;

	msg = NULL;
	if (!frk)
		return (NULL);
	asprintf(&msg, g_msg_cmd[16], frk->name, frk->mom, frk->pos.x, frk->pos.y);
	return (msg);
}
