/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:33:42 by meyu              #+#    #+#             */
/*   Updated: 2025/08/08 15:34:42 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s && s[len])
		len++;
	return (len);
}

char	*ft_strdup(const char *s1)
{
	size_t	l;
	size_t	i;
	char	*s2;

	l = 0;
	while (s1[l])
		l++;
	s2 = (char *)malloc (l + 1);
	if (!s2)
		return (NULL);
	i = 0;
	while (i <= l)
	{
		s2[i] = s1[i];
		i++;
	}
	return (s2);
}

char	*ft_strchr(const char *s, int c)
{
	char	*r;

	r = NULL;
	while (*s)
	{
		if (*s == (char)c)
			r = (char *)s;
		s++;
	}
	if (*s == (char)c)
		r = (char *)s;
	return (r);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*str;
	size_t	index;

	if (!s1 || !s2)
		return (NULL);
	s1_len = 0;
	s2_len = 0;
	while (s1[s1_len])
		s1_len++;
	while (s2[s2_len])
		s2_len++;
	index = 0;
	str = (char *)malloc(s1_len + s2_len + 1);
	if (!str)
		return (NULL);
	while (*s1)
		str[index++] = *s1++;
	while (*s2)
		str[index++] = *s2++;
	str[index] = '\0';
	return (str);
}
