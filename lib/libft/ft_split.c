/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:24:50 by xin               #+#    #+#             */
/*   Updated: 2025/09/10 15:03:11 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char **s, size_t size)
{
	size_t	i;

	if (!s)
		return ;
	i = 0;
	while (i < size)
	{
		free (s[i]);
		i++;
	}
	free (s);
}

static size_t	ft_get_count(char const *s, char c)
{
	size_t	count;
	int		if_string;

	count = 0;
	if_string = 0;
	while (*s)
	{
		if (*s == c)
			if_string = 0;
		else if (!if_string)
		{
			count ++;
			if_string = 1;
		}
		s++;
	}
	return (count);
}

static int	ft_generate_array(char **array, char const *s, char c)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		len = 0;
		while (s[len] && s[len] != c)
			len++;
		if (len > 0)
		{
			array[i] = ft_substr(s, 0, len);
			if (!array[i])
			{
				ft_free(array, i);
				return (0);
			}
			i++;
			s += len;
		}
	}
	array[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**new_array;

	if (!s)
		return (NULL);
	new_array = (char **)malloc ((ft_get_count(s, c) + 1) * sizeof(char *));
	if (!new_array)
		return (NULL);
	if (!ft_generate_array(new_array, s, c))
		return (NULL);
	return (new_array);
}
