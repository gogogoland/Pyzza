/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ring.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 15:45:44 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/16 14:51:28 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static const char	*srv_ring_msg[] =
{
	"Malloc failed for set to ring.\n",
	"Malloc failed for get in ring.\n",
	"Malloc failed for ring (t_ring *.)\n",
	"Malloc failed for ring->command (**char).\n",
	"No data allocated in ring.\n"
};

void		ring_recv(char *command, t_ring *ring)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	while (i < ring->len && ring->command && ring->command[i])
		i++;
	if (i == ring->len)
		return ;
	len = strlen(command);
	if (!(ring->command[i] = (char *)malloc((len + 1) * sizeof(char))))
	{
		ft_putstr(srv_ring_msg[0]);
		return ;
	}
	j = -1;
	while (++j < len)
		ring->command[i][j] = command[j];
	ring->command[i][j] = '\0';
}

char		*ring_send(t_ring *ring)
{
	int		j;
	int		len;
	char	*command;

	if (!ring || !ring->command)
	{
		ft_putstr(srv_ring_msg[4]);
		return (NULL);
	}
	else if (!ring->command[ring->cur])
		return (NULL);
	len = strlen(ring->command[ring->cur]);
	if (!(command = (char *)malloc((len + 1) * sizeof(char))))
	{
		ft_putstr(srv_ring_msg[1]);
		return (NULL);
	}
	j = -1;
	while (++j < len)
		command[j] = ring->command[ring->cur][j];
	command[j] = '\0';
	bzero(ring->command[ring->cur], len);
	ft_memdel((void **)&ring->command[ring->cur]);
	ring->cur = (ring->cur + 1) % ring->len;
	return (command);
}

t_ring		*ring_init(int len)
{
	t_ring	*ring;

	if (!(ring = (t_ring *)malloc(sizeof(t_ring))))
	{
		ft_putstr(srv_ring_msg[2]);
		return (NULL);
	}
	if (!(ring->command = (char **)malloc(len * sizeof(char *))))
	{
		free(ring);
		ft_putstr(srv_ring_msg[3]);
		return (NULL);
	}
	ring->len = len;
	while (--len >= 0)
		ring->command[len] = NULL;
	ring->cur = 0;
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

void	ring_zero(t_ring *ring)
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
	ring->cur = 0;
}
