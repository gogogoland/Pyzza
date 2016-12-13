/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/06 21:24:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:50:04 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_tabjoin(const char **tab, const char *sep)
{
	int		i;
	char	*str;

	if (!tab)
		return (NULL);
	str = NULL;
	i = -1;
	while (tab[++i])
	{
		if (str != NULL)
			str = ft_strjoin(str, sep);
		str = ft_strjoin(str, tab[i]);
	}
	return (str);
}
