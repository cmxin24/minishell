/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nschneid <nschneid@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 18:54:49 by nschneid          #+#    #+#             */
/*   Updated: 2026/01/13 15:36:13 by nschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stddef.h>

static t_env	*env_create_node(char *str);
static void		env_add_node(t_env **head, t_env *node);

t_env	*ft_init_env(char **envp)
{
	t_env	*head;
	t_env	*node;

	head = NULL;
	while (*envp)
	{
		node = env_create_node(*envp);
		env_add_node(&head, node);
		envp++;
	}
	return (head);
}

static void	env_add_node(t_env **head, t_env *node)
{
	t_env	*tmp;

	if (!(*head))
	{
		*head = node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

static t_env	*env_create_node(char *str)
{
	t_env		*node;
	ptrdiff_t	equals_pos;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node || !ft_strchr(str, '='))
		return (NULL);
	equals_pos = ft_strchr(str, '=') - str;
	if (equals_pos < 0)
		return (NULL);
	node->key = ft_substr(str, 0, (size_t)equals_pos);
	str += equals_pos + 1;
	node->value = ft_strdup(str);
	node->next = NULL;
	return (node);
}
