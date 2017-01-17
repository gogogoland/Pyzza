/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ring_recv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/17 00:46:53 by tbalea            #+#    #+#             */
/*   Updated: 2017/01/17 20:37:18 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	ring_recv_allow(t_cmd cmd, t_ring *ring, int cur)
{
	int	i;
	int	beg;

	//printf("old : %s.\n", ring->command[cur]);
	if ((ring->end == 0 && !(ring->command[cur] =\
					(char *)malloc((cmd.end + 1 - cmd.beg) * sizeof(char))))
		|| (ring->end > 0 && !(ring->command[cur] =\
				(char *)reallocf(ring->command[cur], (strlen(ring->command[cur])\
				+ (cmd.end + 1 - cmd.beg)) * sizeof(char)))))
		return (-1);
	i = 0;
	if (ring->end == 0)
		bzero(ring->command[cur], cmd.end + 1 - cmd.beg);
	beg = strlen(ring->command[cur]);
	//printf("0 : beg = %i, i = %i, cur : %s.\n", beg, i, ring->command[cur]);
	while (i + cmd.beg < cmd.end)
	{
		ring->command[cur][i + beg] = cmd.cmd[i + cmd.beg];
		i++;
	}
	//printf("1 : beg = %i, i = %i, cur : %s.\n", beg, i, ring->command[cur]);
	ring->end = !cmd.cmd[cmd.end] ? cur + 1 : 0;
	ring->command[cur][i + beg] = '\0';
	//printf("2 : beg = %i, r_end = %i, i = %i, cur : %s.\n", beg, ring->end, i, ring->command[cur]);
	return (cur);
}
