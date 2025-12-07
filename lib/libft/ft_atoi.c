/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:18:26 by meyu              #+#    #+#             */
/*   Updated: 2025/07/11 18:05:43 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *str)
{
	int		i;
	int		num;
	int		sign;

	num = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num * sign);
}

/* #include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char *str = "--47";

	printf("%d\n", atoi(str));
	printf("%d\n", ft_atoi(str));

	char *str1 = "47";

	printf("%d\n", atoi(str1));
	printf("%d\n", ft_atoi(str1));

	char *str2 = "+47";

	printf("%d\n", atoi(str2));
	printf("%d\n", ft_atoi(str2));
	
	char *str3 = "\n\n\n  -46\b9 \n5d6";

	printf("%d\n", atoi(str3));
	printf("%d\n", ft_atoi(str3));
}
 */