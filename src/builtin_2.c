/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 02:15:11 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 00:30:01 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static t_env	**env_to_array(t_env *env, int count)
{
	t_env	**arr;
	int		i;

	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	return (arr);
}

static void	ft_print_exported(t_env *env)
{
	t_env	**arr;
	t_env	*temp;
	int		count;
	int		i;

	count = 0;
	temp = env;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	arr = env_to_array(env, count);
	ft_sort_env_array(arr, count);
	i = 0;
	while (i < count)
	{
		printf("declare -x %s=\"%s\"\n", arr[i]->key, arr[i]->value);
		i++;
	}
	free(arr);
}

static int	export_one(char *arg, t_env **env)
{
	char	*equal_sign;
	char	*key;

	if (arg[0] == '-' && arg[1] != '\0')
		return (ft_indentifier_error("export", arg), 2);
	if (!ft_is_valid_identifier(arg))
		return (ft_indentifier_error("export", arg), 1);
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (0);
	if (equal_sign > arg && *(equal_sign - 1) == '+')
	{
		*(equal_sign - 1) = '\0';
		key = arg;
		ft_append_env_value(env, key, equal_sign + 1);
		*(equal_sign - 1) = '+';
	}
	else
	{
		*equal_sign = '\0';
		key = arg;
		ft_set_env_value(env, key, equal_sign + 1);
		*equal_sign = '=';
	}
	return (0);
}

int	ft_export(char **args, t_env **env)
{
	int	i;
	int	status;
	int	ret;

	status = 0;
	if (!args[1])
	{
		ft_print_exported(*env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		ret = export_one(args[i], env);
		if (ret > status)
			status = ret;
		i++;
	}
	return (status);
}
