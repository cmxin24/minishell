/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 02:15:11 by xin               #+#    #+#             */
/*   Updated: 2025/12/09 00:19:40 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_export(char **args, t_env **env)
{
	int		i;
	char	*key;
	char	*equal_sign;
	int		exit_status;

	exit_status = 0;
	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (!ft_is_valid_identifier(args[i]))
		{
			ft_indentifier_error("export", args[i]);
			exit_status = 1;
		}
		else
		{
			equal_sign = ft_strchr(args[i], '=');
			if (equal_sign)
			{
				*equal_sign = '\0';
				key = args[i];
				ft_set_env_value(env, key, equal_sign + 1);
				*equal_sign = '=';
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
	while (args[i])
	{
		if (!ft_is_valid_identifier(args[i]) || ft_strchr(args[i], '='))
		{
			ft_indentifier_error("unset", args[i]);
			exit_status = 1;
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
		return (ft_env(*env));
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
