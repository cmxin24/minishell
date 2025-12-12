/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 02:15:11 by xin               #+#    #+#             */
/*   Updated: 2025/12/12 19:36:04 by meyu             ###   ########.fr       */
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
	arr = malloc(sizeof(t_env *) * count);
	if (!arr)
		return ;
	temp = env;
	i = 0;
	while (temp)
	{
		arr[i++] = temp;
		temp = temp->next;
	}
	ft_sort_env_array(arr, count);
	i = 0;
	while (i < count)
	{
		printf("declare -x %s=\"%s\"\n", arr[i]->key, arr[i]->value);
		i++;
	}
	free(arr);
}

int	ft_export(char **args, t_env **env)
{
	int		i;
	char	*key;
	char	*equal_sign;
	int		exit_status;

	exit_status = 0;
	if (!args[1])
	{
		ft_print_exported(*env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1] != '\0')
		{
			ft_indentifier_error("export", args[i]);
			exit_status = 2;
		}
		else if (!ft_is_valid_identifier(args[i]))
		{
			ft_indentifier_error("export", args[i]);
			exit_status = 1;
		}
		else
		{
			equal_sign = ft_strchr(args[i], '=');
			if (equal_sign)
			{
				if (equal_sign > args[i] && *(equal_sign - 1) == '+')
				{
					*(equal_sign - 1) = '\0';
					key = args[i];
					ft_append_env_value(env, key, equal_sign + 1);
					*(equal_sign - 1) = '+';
				}
				else
				{
					*equal_sign = '\0';
					key = args[i];
					ft_set_env_value(env, key, equal_sign + 1);
					*equal_sign = '=';
				}
			}
		}
		i++;
	}
	return (exit_status);
}

int	ft_unset(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1] != '\0')
		{
			ft_indentifier_error("unset", args[i]);
			exit_status = 2;
		}
		else
			ft_unset_env(env, args[i]);
		i++;
	}
	return (exit_status);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **args, t_env **env)
{
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (ft_cd(args, env));
	if (ft_strncmp(args[0], "exit", 5) == 0)
		return (ft_exit(args, *env));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (ft_env(*env, args));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (ft_export(args, env));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (ft_unset(args, env));
	if (ft_strncmp(args[0], "echo", 5) == 0)
		return (ft_echo(args));
	return (0);
}

int	echo_n_flag(char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	if (str[1] != 'n')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
