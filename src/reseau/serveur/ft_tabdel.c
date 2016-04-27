/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/13 14:56:17 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/27 18:15:57 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_tabdel(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
		i++;
	while (--i >= 0)
		ft_memdel((void **)&tab[i]);
	free(tab);
	tab = NULL;
}
