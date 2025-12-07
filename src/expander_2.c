/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 19:44:00 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 16:01:56 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int  get_var_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char *expand_token_str(char *str, t_env **env)
{
	char	*result;
	char	*temp;
	char	*var_key;
	char	*var_value;
	char	*env_val;
	int		i;
	int		start;
	int		in_single_quote;

	result = ft_strdup("");
	i = 0;
	start = 0;
	in_single_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			in_single_quote = !in_single_quote;
		if (str[i] == '$' && !in_single_quote && str[i + 1] && str[i + 1] != ' ')
		{
			temp = ft_substr(str, start, i - start);
			char *new_result = ft_strjoin(result, temp);
			free(result); free(temp);
			result = new_result;
			i++;
			int var_len = get_var_len(&str[i]);
			if (var_len == 1 && str[i] == '?')
				var_value = ft_itoa(0); //TODO: replace 0 with actual exit status
			else
			{
				var_key = ft_substr(str, i, var_len);
				env_val = ft_get_env_value(*env, var_key);
				if (env_val)
					var_value = ft_strdup(env_val);
				else
					var_value = ft_strdup("");
				free(var_key);
			}
			new_result = ft_strjoin(result, var_value);
			free(result); free(var_value);
			result = new_result;
			i += var_len;
			start = i;
		}
		else
			i++;
	}
	temp = ft_substr(str, start, i - start);
	char *final_result = ft_strjoin(result, temp);
	free(result); free(temp);
	return (final_result);
}
