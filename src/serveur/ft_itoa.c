/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/01 03:50:24 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:49:12 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	len_i(int i)
{
	int	len;
	int	a;

	a = i;
	len = (i == 0) ? 1 : 0;
	while (i != 0 && ++len)
		i /= 10;
	return (len);
}

char		*ft_itoa(int i)
{
	char	*str;
	int		neg;
	int		len;

	if (i == -2147483648)
	{
		if ((str = ft_strnew(11)) == NULL)
			return (NULL);
		str = "-2147483648";
		return (str);
	}
	neg = (i < 0) ? 1 : 0;
	len = len_i(i);
	if ((str = ft_strnew(neg + len)) == NULL)
		return (NULL);
	if (neg)
		str[0] = '-';
	str[neg + len] = '\0';
	while (len > 0)
	{
		str[--len + neg] = i % 10 + '0';
		i /= 10;
	}
	return (str);
}
