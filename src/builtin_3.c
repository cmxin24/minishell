/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 21:28:10 by xin               #+#    #+#             */
/*   Updated: 2026/01/17 15:22:03 by meyu             ###   ########.fr       */
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

static int	is_overflow(char *str)
{
	int		len;
	char	*max;
	int		neg;

	neg = 0;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			neg = 1;
	while (*str == '0' && *(str + 1))
		str++;
	len = ft_strlen(str);
	if (len > 19)
		return (1);
	if (len < 19)
		return (0);
	if (neg)
		max = "9223372036854775808";
	else
		max = "9223372036854775807";
	return (ft_strcmp(str, max) > 0);
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
	if (is_overflow(str))
		return (0);
	return (1);
}

int	cd_get_arg_index(char **args, int *idx)
{
	*idx = 1;
	if (args[1] && ft_strcmp(args[1], "--") == 0)
	{
		(*idx)++;
		if (args[*idx] && args[*idx + 1])
			return (ft_putstr_fd(
					"minishell: cd: too many arguments\n", 2), 1);
	}
	else if (args[1] && args[2])
		return (ft_putstr_fd(
				"minishell: cd: too many arguments\n", 2), 1);
	return (0);
}

int	cd_get_target_path(char **args, t_env **env, char **path, int *print_path)
{
	char	*tmp;
	int		idx;

	tmp = NULL;
	*print_path = 0;
	if (cd_get_arg_index(args, &idx))
		return (1);
	if (args[idx] == NULL)
	{
		*path = ft_get_env_value(*env, "HOME");
		if (*path == NULL || **path == '\0')
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else if (idx == 1 && ft_strcmp(args[idx], "-") == 0)
	{
		tmp = ft_get_env_value(*env, "OLDPWD");
		if (tmp == NULL)
			return (ft_putstr_fd(
					"minishell: cd: OLDPWD not set\n", 2), 1);
		*path = ft_strdup(tmp);
		*print_path = 1;
	}
	else
		*path = args[idx];
	return (0);
}
