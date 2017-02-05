/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ring_recv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/17 00:46:53 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/05 19:07:02 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	ring_recv_allow(t_cmd cmd, t_ring *ring, int cur)
{
	int	i;
	int	beg;

	if ((ring->end == 0 && !(ring->command[cur] =\
					(char *)malloc((cmd.end + 1 - cmd.beg) * sizeof(char))))
		|| (ring->end > 0 && !(ring->command[cur] = (char *)reallocf(\
				ring->command[cur], (strlen(ring->command[cur])\
				+ (cmd.end + 1 - cmd.beg)) * sizeof(char)))))
		return (-1);
	i = 0;
	if (ring->end == 0)
		bzero(ring->command[cur], cmd.end + 1 - cmd.beg);
	beg = strlen(ring->command[cur]);
	while (i + cmd.beg < cmd.end)
	{
		ring->command[cur][i + beg] = cmd.cmd[i + cmd.beg];
		i++;
	}
	ring->end = !cmd.cmd[cmd.end] ? cur + 1 : 0;
	ring->command[cur][i + beg] = '\0';
	return (cur);
}
