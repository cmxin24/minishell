/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nschneid <nschneid@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 20:55:36 by nschneid          #+#    #+#             */
/*   Updated: 2026/01/05 21:01:38 by nschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	get_env_count(t_env *head)
{
	size_t	count;

	count = 0;
	while(head)
	{
		count++;
		head = head->next;
	}
	return (count);
}

char	*env_to_str(t_env *node)
{
	char	*str;

	str = ft_strjoin(node->key, "=");
	str = ft_strjoin(str, node->value);
	return (str);
}
