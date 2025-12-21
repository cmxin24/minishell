/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 20:18:09 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 18:14:25 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_heredoc_line(char *line, t_env *env)
{
	char	*res;
	char	*temp;
	char	*key;
	char	*val;
	int		i;
	int		start;
	int		len;
	char	*join1;
	char	*join2;
	char	*final;
	char	*env_v;

	res = ft_strdup("");
	i = 0;
	start = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && line[i + 1] != ' ')
		{
			temp = ft_substr(line, start, i - start);
			join1 = ft_strjoin(res, temp);
			free(res);
			free(temp);
			res = join1;
			i++;
			len = 0;
			if (line[i] == '?')
				len = 1;
			else
				while (line[i + len] && (ft_isalnum(line[i + len])
						|| line[i + len] == '_'))
					len++;
			if (len == 1 && line[i] == '?')
				val = ft_itoa(g_signal);
			else
			{
				key = ft_substr(line, i, len);
				env_v = ft_get_env_value(env, key);
				if (env_v)
					val = ft_strdup(env_v);
				else
					val = ft_strdup("");
				free(key);
			}
			join2 = ft_strjoin(res, val);
			free(res);
			free(val);
			res = join2;
			i += len;
			start = i;
		}
		else
			i++;
	}
	temp = ft_substr(line, start, i - start);
	final = ft_strjoin(res, temp);
	free(res);
	free(temp);
	return (final);
}

/**
 * @brief restore normal signal handlers
 */
void	ft_restore_signals(void)
{
	rl_catch_signals = 1;
	ft_init_signals();
}
