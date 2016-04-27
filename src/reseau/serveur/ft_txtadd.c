/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_txtadd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/27 13:38:09 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/27 18:40:07 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_txtadd(char **tab, char *elm)
{
	char	**new;
	int		i;
	int		max;

	i = 0;
	while (tab && tab[i])
		i++;
	if (!(new = (char **)malloc((i + 1) * sizeof(char *))))
		return (NULL);
	max = ft_strlen(elm) + 2;
	if (!(new[i] = (char *)malloc((max - 1) * sizeof(char))))
		return (NULL);
	while (max-- > 0)
		new[i][max] = elm[max];
	new[i+1] = NULL;
	while (--i >= 0)
	{
		max = ft_strlen(tab[i]) + 2;
		if (!(new[i] = (char *)malloc((max - 1) * sizeof(char))))
			return (NULL);
		while (max-- > 0)
			new[i][max] = tab[i][max];
	}
	ft_tabdel(tab);
	return (new);
}
