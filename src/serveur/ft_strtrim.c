/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/30 20:25:16 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:49:59 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	begining(char const *str)
{
	int	i;

	i = -1;
	while (str[++i] != '\0'
			&& (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'))
		;
	i--;
	return (i);
}

static char	*strmem(char const *str)
{
	int		i;
	int		len;
	int		flag;
	int		chang;
	char	*res;

	if (!str)
		return (NULL);
	i = begining(str);
	len = 1;
	flag = 1;
	chang = 1;
	while (str[++i] != '\0')
	{
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			flag = 0;
		else
			flag = 1;
		if (flag || chang != flag)
			len++;
		chang = flag;
	}
	if (str[--i] == ' ' || str[i] == '\n' || str[i] == '\t')
		len--;
	return ((res = (char *)malloc(len * sizeof(char))));
}

static void	loop(char const *str, char *res, int i)
{
	int		j;
	int		flag;
	int		chang;

	j = -1;
	flag = 1;
	chang = 1;
	while (str[++i] != '\0')
	{
		if ((str[i] == ' ' || str[i] == '\n' || str[i] == '\t'))
			flag = 0;
		else
			flag = 1;
		if ((flag || chang != flag) && ++j >= 0)
			res[j] = str[i];
		chang = flag;
	}
	if (res[j] == ' ' || res[j] == '\n' || res[j] == '\t')
		res[j] = '\0';
	else
		res[j + 1] = '\0';
}

char		*ft_strtrim(char const *str)
{
	int		i;
	char	*res;

	if (!(res = strmem(str)))
		return (NULL);
	i = begining(str);
	loop(str, res, i);
	return (res);
}
