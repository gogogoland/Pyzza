/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/04/15 18:03:32 by tbalea            #+#    #+#             */
/*   Updated: 2016/10/01 19:54:30 by tbalea           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>

void				ft_bzero(void *v, size_t s);
void				*ft_memalloc(size_t size);
void				*ft_memcpy(void *s1, const void *s2, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
void				ft_memdel(void **del);
void				ft_tabdel(char **tab);
void				ft_tabdelelem(char **tab, size_t i);
void				*ft_memset(void *mem, int c, size_t size);
size_t				ft_strlen(const char *str);
char				*ft_strdup(const char *s1);
char				*ft_strbdup(const char *str, size_t b);
void				ft_strdelelem(char *str, size_t i);
char				*ft_strcat(char *s1, const char *s2);
char				*ft_strncat(char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnew(size_t size);
char				**ft_strsplit(const char *s, const char c);
char				*ft_tabjoin(const char **tab, const char *sep);
int					ft_atoi(const char *str);
int					ft_isempty(char c);
int					ft_isdigit(int c);
int					ft_strisdigit(char const *str);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strcjoin(char const *s1, char const *s2, const char c);
char				*ft_strtrim(char const *s);
char				*ft_itoa(int n);
void				ft_putchar(char c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);
char				*ft_strdelvoid(char *str);
void				ft_resizestr(char **str, int empty, int fill);
char				**ft_txtadd(char **tab, char *elm);

#endif
