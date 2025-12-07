/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 11:16:26 by meyu              #+#    #+#             */
/*   Updated: 2025/07/11 17:13:04 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_number_len(long n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	generate_str(char *str, long m, int len)
{
	if (m == 0)
		str[0] = '0';
	while (m > 0)
	{
		str[--len] = (m % 10) + '0';
		m /= 10;
	}
}

char	*ft_itoa(int n)
{
	long	m;
	int		sign;
	char	*str;
	int		len;

	m = n;
	sign = (m < 0);
	if (sign)
		m = -m;
	len = ft_number_len(m);
	str = (char *)malloc(len + sign + 1);
	if (!str)
		return (NULL);
	str[len + sign] = '\0';
	if (sign)
		str[0] = '-';
	generate_str(str + sign, m, len);
	return (str);
}
