/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:18:09 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 00:58:00 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*join_prefix(char *res, char *line, int start, int i)
{
	char	*temp;
	char	*new_res;

	temp = ft_substr(line, start, i - start);
	new_res = ft_strjoin(res, temp);
	free(res);
	free(temp);
	return (new_res);
}

static char	*get_heredoc_var_value(char *line, int *i, t_env *env)
{
	int		len;
	char	*key;
	char	*val;
	char	*env_v;

	if (line[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(g_signal));
	}
	len = 0;
	while (line[*i + len] && (ft_isalnum(line[*i + len])
			|| line[*i + len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup(""));
	key = ft_substr(line, *i, len);
	env_v = ft_get_env_value(env, key);
	if (env_v)
		val = ft_strdup(env_v);
	else
		val = ft_strdup("");
	free(key);
	*i += len;
	return (val);
}

static char	*join_var(char *res, char *val)
{
	char	*new_res;

	new_res = ft_strjoin(res, val);
	free(res);
	free(val);
	return (new_res);
}

char	*expand_heredoc_line(char *line, t_env *env)
{
	char	*res;
	int		i;
	int		start;

	if (!line)
		return (NULL);
	res = ft_strdup("");
	i = 0;
	start = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && line[i + 1] != ' ')
		{
			res = join_prefix(res, line, start, i);
			i++;
			res = join_var(res, get_heredoc_var_value(line, &i, env));
			start = i;
		}
		else
			i++;
	}
	res = join_prefix(res, line, start, i);
	return (res);
}

/**
 * @brief restore normal signal handlers
 */
void	ft_restore_signals(void)
{
	rl_catch_signals = 1;
	ft_init_signals();
}
