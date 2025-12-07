/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:12:29 by xin               #+#    #+#             */
/*   Updated: 2025/07/08 19:21:22 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

/* #include <stdio.h>
#include <string.h>

int main ()
{
   const char str[] = "https://www.runoob.com";
   const char ch = 'x';
   char *ptr;
   char *ft_ptr;

   ptr = strchr(str, ch);
   ft_ptr = ft_strchr(str, ch);

    printf("%s\n", ptr);
	printf("%s\n", ft_ptr);
   return(0);
}
 */