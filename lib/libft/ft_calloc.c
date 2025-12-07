/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:39:17 by xin               #+#    #+#             */
/*   Updated: 2025/07/11 18:00:03 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void			*ptr;
	size_t			i;
	unsigned char	*m;

	ptr = (void *)malloc(count * size);
	if (!ptr)
		return (NULL);
	i = 0;
	m = (unsigned char *)ptr;
	while (i < count * size)
	{
		m[i] = 0;
		i++;
	}
	return (ptr);
}
