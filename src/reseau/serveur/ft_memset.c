/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/29 19:23:21 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:49:26 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *mem, int c, size_t size)
{
	unsigned char	*str;

	str = (unsigned char *)(mem);
	while (size--)
	{
		*str = (unsigned char)(c);
		str++;
	}
	return (mem);
}
