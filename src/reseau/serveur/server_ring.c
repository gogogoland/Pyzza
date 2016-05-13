/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ring.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/03 15:45:44 by tbalea            #+#    #+#             */
/*   Updated: 2016/05/11 15:21:51 by tbalea           ###   ########.fr       */
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

ft_putendl("server_ring.c	ring_recv	0");
	i = 0;
	while (i < ring->len && ring->command && ring->command[i])
		i++;
ft_putendl("server_ring.c	ring_recv	1");
	if (i == ring->len)
		return ;
ft_putendl("server_ring.c	ring_recv	2");
	len = strlen(command);
	if (!(ring->command[i] = (char *)malloc((len + 1) * sizeof(char))))
	{
		ft_putstr(srv_ring_msg[0]);
		return ;
	}
ft_putendl("server_ring.c	ring_recv	3");
	j = -1;
	while (++j < len)
		ring->command[i][j] = command[j];
	ring->command[i][j] = '\0';
ft_putendl("server_ring.c	ring_recv	4");
}

char		*ring_send(t_ring *ring)
{
	int		j;
	int		len;
	char	*command;

ft_putendl("server_ring.c	ring_send	0");
	if (!ring || !ring->command)
	{
		ft_putstr(srv_ring_msg[4]);
ft_putendl("server_ring.c	ring_send	1");
		return (NULL);
	}
	else if (!ring->command[ring->cur])
		return (NULL);
ft_putendl("server_ring.c	ring_send	2");
	len = strlen(ring->command[ring->cur]);
	if (!(command = (char *)malloc((len + 1) * sizeof(char))))
	{
		ft_putstr(srv_ring_msg[1]);
		return (NULL);
	}
ft_putendl("server_ring.c	ring_send	3");
	j = -1;
	while (++j < len)
		command[j] = ring->command[ring->cur][j];
ft_putendl("server_ring.c	ring_send	4");
	command[j] = '\0';
	bzero(ring->command[ring->cur], len);
	ft_memdel((void **)&ring->command[ring->cur]);
	ring->cur = (ring->cur + 1) % ring->len;
ft_putendl("server_ring.c	ring_send	5");
	return (command);
}

t_ring		*ring_init(int len)
{
	t_ring	*ring;

ft_putendl("server_ring.c	ring_init	0");
	if (!(ring = (t_ring *)malloc(sizeof(t_ring))))
	{
		ft_putstr(srv_ring_msg[2]);
		return (NULL);
	}
ft_putendl("server_ring.c	ring_init	1");
	if (!(ring->command = (char **)malloc(len * sizeof(char *))))
	{
		free(ring);
		ft_putstr(srv_ring_msg[3]);
		return (NULL);
	}
ft_putendl("server_ring.c	ring_init	2");
	ring->len = len;
	while (--len >= 0)
		ring->command[len] = NULL;
	ring->cur = 0;
ft_putendl("server_ring.c	ring_init	3");
	return (ring);
}

void		ring_kill(t_ring *ring)
{
ft_putendl("server_ring.c	ring_kill	0");
	if (!ring)
		return ;
ft_putendl("server_ring.c	ring_kill	1");
	while (--ring->len >= 0)
	{
ft_putendl("server_ring.c	ring_kill	2");
		if (!ring->command[ring->len])
			continue ;
ft_putendl("server_ring.c	ring_kill	3");
		bzero(ring->command[ring->len], strlen(ring->command[ring->len]));
		free(ring->command[ring->len]);
	}
ft_putendl("server_ring.c	ring_kill	4");
	ring->cur = 0;
	free(ring->command);
	ring->command = NULL;
	free(ring);
	ring = NULL;
ft_putendl("server_ring.c	ring_kill	5");
}

void	ring_zero(t_ring *ring)
{
	int	i;

	i = -1;
ft_putendl("server_ring.c	ring_zero	0");
	while (ring && ring->command && ++i < ring->len)
	{
ft_putendl("server_ring.c	ring_zero	1");
		if (ring->command[i])
		{
ft_putendl("server_ring.c	ring_zero	2");
			bzero(ring->command[i], strlen(ring->command[i]));
			free(ring->command[i]);
			ring->command[i] = NULL;
ft_putendl("server_ring.c	ring_zero	3");
		}
	}
ft_putendl("server_ring.c	ring_zero	4");
	ring->cur = 0;
}
