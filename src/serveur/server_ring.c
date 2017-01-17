/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ring.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 15:45:44 by tbalea            #+#    #+#             */
/*   Updated: 2017/01/17 20:34:04 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*g_srv_ring_msg[] =
{
	"Malloc failed for set to ring.\n",
	"Malloc failed for get in ring.\n",
	"Malloc failed for ring (t_ring *.)\n",
	"Malloc failed for ring->command (**char).\n",
	"No data allocated in ring.\n",
	"command from socket %i : %s\n",
	"command from player #%i : %s\n",
	"No more space for new command\n"
};

void		ring_recv(t_server *srv, t_cmd cmd, t_ring *ring, int who)
{
	char	*log;
	int		limit;
	int		i;
	int		len;

	log = NULL;
	limit = -1;
	i = ring->cur;
	while (++limit < ring->len && ring->command && ring->command[i])
		i = (i + 1) % ring->len;
	i = ring->end ? i + (ring->len * !i) - 1 : i;
	//if (ring->end != 0)
	//	i = (!i ? ring->len : i) - 1;
	//printf("end = %i, i = %i. len = %i\n", ring->end, i, ring->len);
	if (cmd.end - cmd.beg == 0)
		return ;
	if ((limit == ring->len && ring->end == 0)
			|| ((i = ring_recv_allow(cmd, ring, i)) < 0))
	{
		i >= 0 ? server_log(srv, g_srv_ring_msg[7])
			: server_log(srv, g_srv_ring_msg[0]);
		return ;
	}
	!ring->end && asprintf(&log, g_srv_ring_msg[5 + (who > 0)], who * ((who > 0)
				- (who < 0)), ring->command[i]) > 0 ? server_log(srv, log) : 0;
	//printf("end = %i, cur = %i.\n", ring->end, ring->cur);
	log ? ft_memdel((void **)&log) : 0;
}

char		*ring_send(t_server *srv, t_ring *ring)
{
	int		j;
	int		len;
	char	*cmd;

	if (!ring || !ring->command)
	{
		server_log(srv, g_srv_ring_msg[4]);
		return (NULL);
	}
	else if (!ring->command[ring->cur] || ring->end == ring->cur + 1)
		return (NULL);
	len = strlen(ring->command[ring->cur]);
	if (!(cmd = (char *)malloc((len + 1) * sizeof(char))))
	{
		server_log(srv, g_srv_ring_msg[1]);
		return (NULL);
	}
	j = -1;
	while (++j < len)
		cmd[j] = ring->command[ring->cur][j];
	cmd[j] = '\0';
	bzero(ring->command[ring->cur], len);
	ft_memdel((void **)&ring->command[ring->cur]);
	ring->cur = (ring->cur + 1) % ring->len;
	return (cmd);
}

t_ring		*ring_init(t_server *srv, int len)
{
	t_ring	*ring;

	if (!(ring = (t_ring *)malloc(sizeof(t_ring))))
	{
		server_log(srv, g_srv_ring_msg[2]);
		return (NULL);
	}
	if (!(ring->command = (char **)malloc(len * sizeof(char *))))
	{
		free(ring);
		server_log(srv, g_srv_ring_msg[3]);
		return (NULL);
	}
	ring->len = len;
	while (--len >= 0)
		ring->command[len] = NULL;
	ring->cur = 0;
	ring->end = 0;
	return (ring);
}

void		ring_kill(t_ring *ring)
{
	if (!ring)
		return ;
	while (--ring->len >= 0)
	{
		if (!ring->command[ring->len])
			continue ;
		bzero(ring->command[ring->len], strlen(ring->command[ring->len]));
		free(ring->command[ring->len]);
	}
	ring->cur = 0;
	free(ring->command);
	ring->command = NULL;
	free(ring);
	ring = NULL;
}

void		ring_zero(t_ring *ring)
{
	int	i;

	i = -1;
	while (ring && ring->command && ++i < ring->len)
	{
		if (ring->command[i])
		{
			bzero(ring->command[i], strlen(ring->command[i]));
			free(ring->command[i]);
			ring->command[i] = NULL;
		}
	}
	ring->end = 0;
	ring->cur = 0;
}
