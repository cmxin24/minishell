/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 22:12:26 by xin               #+#    #+#             */
/*   Updated: 2025/07/11 18:12:55 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check_len(size_t s_len, unsigned int start)
{
	if (start >= s_len)
		return (0);
	return (1);
}

static size_t	ft_count_len(size_t s_len, size_t len, unsigned int start)
{
	if (s_len - start < len)
		return (s_len - start);
	return (len);
}

static void	copy(char *subs, const char *s, size_t subs_len, unsigned int start)
{
	size_t	i;

	i = 0;
	while (i < subs_len)
	{
		subs[i] = s[start + i];
		i++;
	}
	subs[subs_len] = '\0';
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	subs_len;
	char	*subs;
	int		check;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	check = ft_check_len(s_len, start);
	if (check == 0)
	{
		subs = malloc(1);
		if (!subs)
			return (NULL);
		subs[0] = '\0';
		return (subs);
	}
	subs_len = ft_count_len(s_len, len, start);
	subs = malloc(subs_len + 1);
	if (!subs)
		return (NULL);
	copy(subs, s, subs_len, start);
	return (subs);
}
