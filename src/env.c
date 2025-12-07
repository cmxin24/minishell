/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 19:33:29 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 15:00:35 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*ft_new_env_node(char *str)
{
	t_env	*new_node;
	int		i;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	new_node->key = ft_substr(str, 0, i);
	if (str[i] == '=')
		new_node->value = ft_strdup(str + i + 1);
	else
		new_node->value = ft_strdup("");
	new_node->next = NULL;
	return (new_node);
}

t_env *ft_init_env(char **envp)
{
	t_env	*list;
	t_env	*current;
	t_env	*new;
	int		i;

	list = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		new = ft_new_env_node(envp[i]);
		if (!list)
		{
			list = new;
			current = list;
		}
		else
		{
			current->next = new;
			current = current->next;
		}
		i++;
	}
	return (list);
}

char **ft_env_list_to_array(t_env *env)
{
	char	**arr;
	t_env	*temp;
	int		count;
	int		i;
	char	*temp_str;

	count = 0;
	temp = env;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	temp = env;
	i = 0;
	while (temp)
	{
		temp_str = ft_strjoin(temp->key, "=");
		arr[i] = ft_strjoin(temp_str, temp->value);
		free(temp_str);
		temp = temp->next;
		i++;
	}
	return (arr[i] = NULL, arr);
}

char *ft_get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
