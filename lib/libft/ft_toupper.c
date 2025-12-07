/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 00:15:46 by xin               #+#    #+#             */
/*   Updated: 2025/07/08 18:06:43 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c >= 97 && c <= 122)
		c -= 32;
	return (c);
}

/* #include <ctype.h>
#include <stdio.h>

int	main(void)
{
	int	c = 96;
	printf ("%d\n", ft_toupper(c));
	printf ("%d\n", toupper(c));

	c = 97;
	printf ("%d\n", ft_toupper(c));
	printf ("%d\n", toupper(c));

	c = 65;
	printf ("%d\n", ft_toupper(c));
	printf ("%d\n", toupper(c));
}
 */