/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nschneid <nschneid@student.42heilbronn.de  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 18:54:49 by nschneid          #+#    #+#             */
/*   Updated: 2026/01/05 21:02:38 by nschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_get_env_value(t_env *head, char *key)
{
	while(head)
	{
		if (ft_strcmp(head->key, key) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

void	ft_set_env_value(t_env **head, char *key, char *value)
{
	t_env	*tmp;

	tmp = *head;
	while(tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = value;
			return ;
		}
		tmp = tmp->next;
	}
	tmp = (t_env *)malloc(sizeof(t_env));
	if (!tmp)
		return ;
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(value);
	tmp->next = NULL;
	*head = tmp;
	return ;
}

void	ft_append_env_value(t_env **head, char *key, char *value)
{
	t_env	*tmp;
	char	*str;

	tmp = *head;
	while(tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			str = ft_strjoin(tmp->value, value);
			free(tmp->value);
			tmp->value = str;
			return ;
		}
		tmp = tmp->next;
	}
	tmp = (t_env *)malloc(sizeof(t_env));
	if (!tmp)
		return ;
	tmp->key = ft_strdup(key);
	tmp->value = ft_strdup(value);
	tmp->next = NULL;
	*head = tmp;
	return ;
}

void	ft_unset_env(t_env **head, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (!prev)
			{
				*head = curr->next;
				return ;
			}
			prev->next = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	return ;
}

char	**ft_env_list_to_array(t_env *head)
{
	size_t	count;
	char	**arr;

	count = get_env_count(head);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr[count] = NULL;
	count = 0;
	while(head)
	{
		arr[count] = env_to_str(head);
		printf("%s\n", arr[count]);
		count++;
		head = head->next;
	}
	return (arr);
}
