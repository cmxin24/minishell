/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 02:14:20 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 12:53:55 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char **args, t_env *env, int print)
{
	int	exit_code;

	exit_code = 0;
	if (print)
		printf("exit\n");
	if (args[1])
	{
		if (!ft_check_exit(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		else if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		exit_code = ft_atoi(args[1]) % 256;
		ft_free_env_list(env);
		exit(exit_code);
	}
	ft_free_env_list(env);
	return (exit(exit_code), 0);
}

int	ft_cd(char **args, t_env **env)
{
	char	*path;
	int		print_path;
	char	cwd[1024];

	path = NULL;
	if (cd_get_target_path(args, env, &path, &print_path) != 0)
		return (1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_set_env_value(env, "OLDPWD", cwd);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		if (print_path)
			free(path);
		return (1);
	}
	if (print_path)
	{
		printf("%s\n", path);
		free(path);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_set_env_value(env, "PWD", cwd);
	return (0);
}

int	ft_env(t_env *env, char **args)
{
	if (args[1])
	{
		ft_putstr_fd("env: ‘", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("’: No such file or directory\n", 2);
		return (127);
	}
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	ft_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && echo_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
