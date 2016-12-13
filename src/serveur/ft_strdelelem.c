/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdelelem.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/08 10:48:43 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:49:45 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strdelelem(char *str, size_t i)
{
	if (!str || i > ft_strlen(str))
		return ;
	while (str[++i])
		str[i - 1] = str[i];
	str[i - 1] = '\0';
}
