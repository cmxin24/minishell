/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 20:55:36 by nschneid          #+#    #+#             */
/*   Updated: 2026/01/18 16:45:12 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	get_env_count(t_env *head)
{
	size_t	count;

	count = 0;
	while (head)
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

static t_env	*ft_get_or_create_env(t_env **head, char *key, char *value)
{
	t_env	*tmp;
	t_env	*last;

	tmp = *head;
	last = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		last = tmp;
		tmp = tmp->next;
	}
	tmp = (t_env *)malloc(sizeof(t_env));
	if (!tmp)
		return (NULL);
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(value);
	tmp->next = NULL;
	if (last)
		last->next = tmp;
	else
		*head = tmp;
	return (tmp);
}

void	ft_set_env_value(t_env **head, char *key, char *value)
{
	t_env	*node;

	node = ft_get_or_create_env(head, key, value);
	if (!node)
		return ;
	free(node->value);
	node->value = ft_strdup(value);
}

void	ft_append_env_value(t_env **head, char *key, char *value)
{
	t_env	*node;
	char	*str;

	node = ft_get_or_create_env(head, key, value);
	if (!node)
		return ;
	str = ft_strjoin(node->value, value);
	free(node->value);
	node->value = str;
}
