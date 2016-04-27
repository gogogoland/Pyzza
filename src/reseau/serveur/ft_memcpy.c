/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/28 14:38:31 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:49:23 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *s1, const void *s2, size_t n)
{
	void	*str;

	str = s1;
	while (n)
	{
		*((char *)s1) = *((char *)s2);
		s1 = (void *)((char *)s1 + 1);
		s2 = (void *)((char *)s2 + 1);
		n--;
	}
	return (str);
}
