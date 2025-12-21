/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 19:33:29 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 17:26:17 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief copy a new environment variable node from "KEY=VALUE"
 * @note 
 * KEY will never have '=', but value can: VAR=hello=world
 * VALUE can be an empty string, but not NULL
 */
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

/**
 * @brief get SHLVL from env then +1, if not exist, set it to 1
 * @note
 * SHLVL value may bigger than 9, is a string not char
 */
static void	ft_set_shlvl(t_env	*list)
{
	char	*shlvl_value;
	int		shlvl_num;
	char	*new_shlvl;

	shlvl_value = ft_get_env_value(list, "SHLVL");
	if (shlvl_value)
	{
		shlvl_num = ft_atoi(shlvl_value);
		shlvl_num++;
		new_shlvl = ft_itoa(shlvl_num);
		ft_set_env_value(&list, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
	else
		ft_set_env_value(&list, "SHLVL", "1");
}

/**
 * @brief copy an environment variable linked list from system envp
 * @note
 * the different is must set SHLVL(shell level) +1
 */
t_env	*ft_init_env(char **env)
{
	t_env	*list;
	t_env	*current;
	t_env	*new;
	int		i;

	list = NULL;
	current = NULL;
	i = 0;
	while (env[i])
	{
		new = ft_new_env_node(env[i]);
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
	ft_set_shlvl(list);
	return (list);
}

char	**ft_env_list_to_array(t_env *env, int i)
{
	char	**array;
	t_env	*temp;
	int		count;
	char	*temp_str;

	count = 0;
	temp = env;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	temp = env;
	while (temp)
	{
		temp_str = ft_strjoin(temp->key, "=");
		array[i] = ft_strjoin(temp_str, temp->value);
		free(temp_str);
		temp = temp->next;
		i++;
	}
	return (array[i] = NULL, array);
}

char	*ft_get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
