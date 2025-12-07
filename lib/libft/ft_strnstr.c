/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:56:08 by xin               #+#    #+#             */
/*   Updated: 2025/07/11 17:37:02 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	a;
	size_t	b;

	if (!haystack && len == 0)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	a = 0;
	while (haystack[a] != '\0' && a < len)
	{
		b = 0;
		while ((a + b) < len && haystack[a + b] && haystack[a + b] == needle[b])
		{
			b++;
		}
		if (needle[b] == '\0')
			return ((char *)(haystack + a));
		a++;
	}
	return (NULL);
}
