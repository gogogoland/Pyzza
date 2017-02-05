/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdelelem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 17:10:19 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:50:02 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_tabdelelem(char **tab, size_t i)
{
	size_t		k;

	k = 0;
	while (tab && tab[k])
		k++;
	if (k < i || !tab)
		return ;
	k = i;
	while (tab[++k])
	{
		ft_memdel((void **)&tab[i]);
		tab[i] = ft_strdup(tab[k]);
		i++;
	}
	tab[i] = NULL;
}
