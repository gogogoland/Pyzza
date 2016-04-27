/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/06/01 00:24:04 by tbalea            #+#    #+#             */
/*   Updated: 2016/04/09 15:49:58 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	len_str(const char *str, const char c, int b)
{
	while (str && str[b] != c && str[b] != '\0')
		b++;
	return (b);
}

static char	*add_str(const char *str, const char c, int t)
{
	int		n;
	int		i;
	char	*ret;

	n = 0;
	i = len_str(str, c, t);
	ret = (char *)malloc((i - t + 1) * sizeof(char));
	while (str && t + n < i)
	{
		ret[n] = str[t + n];
		n++;
	}
	ret[n] = '\0';
	return (ret);
}

static int	count_c(const char *str, const char c)
{
	int	i;
	int	n;
	int	s;

	s = 1;
	n = 0;
	i = -1;
	if (str == NULL)
		return (0);
	while (str && str[++i] != '\0')
	{
		if (str[i] == c)
			s = 1;
		else if (s && ++n)
			s = 0;
	}
	return (n);
}

char		**ft_strsplit(const char *str, const char c)
{
	char	**tab;
	int		t;
	int		i;
	int		j;

	i = 0;
	j = 0;
	t = count_c(str, c);
	tab = (char **)malloc((t + 1) * sizeof(char *));
	while (i < t)
	{
		while (str[j] == c)
			j++;
		tab[i] = add_str(str, c, j);
		j += len_str(str, c, j) - j;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
