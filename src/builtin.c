/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 02:14:20 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 16:09:00 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char **args, t_env *env)
{
	int exit_status = 0;

	printf("exit\n");

	if (args[1])
	{
		// exit_status = ft_atoi(args[1]);
	}
	ft_free_env_list(env);
	exit(exit_status);
	return (0);
}

int	ft_cd(char **args, t_env **envp)
{
	char	*path;
	char	cwd[1024];

	if (args[1] == NULL)
	{
		path = ft_get_env_value(*envp, "HOME");
		if (path == NULL || *path == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		path = args[1];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_set_env_value(envp, "OLDPWD", cwd);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_set_env_value(envp, "PWD", cwd);
	return (0);
}

int	ft_env(t_env *envp)
{
	while (envp)
	{
		printf("%s=%s\n", envp->key, envp->value);
		envp = envp->next;
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

