/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/24 14:30:20 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/02 19:46:31 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strisdigit(char const *str)
{
	int	i;

	if (!str)
		return (0);
	i = str[0] == '-' ? 0 : -1;
	while (str[++i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
	}
	if (str[0] == '-' && i == 1)
		return (0);
	return (1);
}
