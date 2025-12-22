/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:29:47 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 15:47:32 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_unset_key(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
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
		else if (is_valid_unset_key(args[i]))
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

int	exec_builtin(char **args, t_env **env, int is_child)
{
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (ft_cd(args, env));
	if (ft_strncmp(args[0], "exit", 5) == 0)
		return (ft_exit(args, *env, !is_child));
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
