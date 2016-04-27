/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/31 16:31:48 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:49:19 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void	*alc;

	alc = NULL;
	if (size)
	{
		alc = (void *)malloc(size * sizeof(void));
		if (alc)
			ft_bzero(alc, size);
	}
	return (alc);
}
