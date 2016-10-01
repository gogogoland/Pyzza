/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_log.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/16 15:13:07 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/01 11:42:06 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

//	TODO
//	*	Save log
void	server_log(t_server *srv, const char *msg)
{
	write(1, msg, strlen(msg));
//	if (srv && srv->log)
//		write(srv->log, msg, strlen(msg));
}
