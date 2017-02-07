/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_time.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/07 17:57:30 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/07 20:11:14 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	server_time(struct timeval *time)
{
	long int		diffsec;
	int				diff;
	struct timeval	renchon;

	diff = 0;
	diffsec = 0;
	diff -= time->tv_usec;
	diffsec -= time->tv_sec;
	gettimeofday(&renchon, NULL);
	diffsec += renchon.tv_sec;
	diff += diffsec * 1000000 + renchon.tv_usec;
	if (diff > 0)
		gettimeofday(time, NULL);
	return (diff > 0 ? diff : 0);
}
