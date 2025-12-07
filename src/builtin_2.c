/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 02:15:11 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 15:59:00 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_export(char **args, t_env **env)
{
	int i;
	char *key;
	char *value;
	char *equal_sign;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			key = args[i];
			value = equal_sign + 1;
			ft_set_env_value(env, key, value);
			*equal_sign = '=';
		}
		i++;
	}
	return (0);
}

int ft_unset(char **args, t_env **env)
{
	int	i;
	
	i = 1;
	while (args[i])
	{
		ft_unset_env(env, args[i]);
		i++;
	}
	return (0);
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
		return (ft_env(*env));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (ft_export(args, env));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (ft_unset(args, env));
	return (0);
}
