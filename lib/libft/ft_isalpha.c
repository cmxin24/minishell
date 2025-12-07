/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 13:00:24 by meyu              #+#    #+#             */
/*   Updated: 2025/07/07 14:17:04 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((65 <= c && c <= 90) || (97 <= c && c <= 122))
		return (1);
	return (0);
}

/* #include <stdio.h>
#include <ctype.h>

int	main(void)
{
	int	is_alpha;

	is_alpha = 0;
	is_alpha = ft_isalpha('a');
	printf("%d\n", is_alpha);
	is_alpha = isalpha('a');
	printf("%d\n", is_alpha);

	return (0);
}
 */