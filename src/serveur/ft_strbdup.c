/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strbdupt.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/04/14 18:29:23 by tbalea            #+#    #+#             */
/*   Updated: 2016/09/28 15:15:18 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strbdup(const char *str, size_t b)
{
	char	*ret;
	size_t	len;
	size_t	i;

	ret = NULL;
	if (!str || (len = ft_strlen(str)) < b)
		return (ret);
	ret = ft_strnew(len - b);
	i = -1;
	while ((++i + b) < len)
		ret[i] = str[b + i];
	ret[i] = '\0';
	return (ret);
}
