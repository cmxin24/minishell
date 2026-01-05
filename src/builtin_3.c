/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:28:10 by xin               #+#    #+#             */
/*   Updated: 2026/01/05 18:06:09 by nschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_valid_identifier(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (1);
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	ft_indentifier_error(char *cmd, char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	ft_check_exit(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	cd_get_target_path(char **args, t_env **env, char **path, int *print_path)
{
	char	*tmp;

	tmp = NULL;
	*print_path = 0;
	if (args[1] && args[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	if (args[1] == NULL)
	{
		*path = ft_get_env_value(*env, "HOME");
		if (*path == NULL || **path == '\0')
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		tmp = ft_get_env_value(*env, "OLDPWD");
		if (tmp == NULL)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
		*path = ft_strdup(tmp);
		*print_path = 1;
	}
	else
		*path = args[1];
	return (0);
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
