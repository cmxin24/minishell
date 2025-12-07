/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:47:12 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 14:57:17 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_len_without_quotes(char *str)
{
	int		len;
	int		i;
	char	quote;

	len = 0;
	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			len++;
		i++;
	}
	return (len);
}

static char	*ft_strip_quotes(char *str, int len)
{
	char	*new_str;
	int		i;
	int		j;
	char	quote;

	if (!str)
		return (NULL);
	len = ft_len_without_quotes(str);
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			new_str[j++] = str[i];
		i++;
	}
	return (new_str[j] = '\0', new_str);
}

void	ft_expander(t_cmd *cmd_list, t_env **envp)
{
	t_cmd	*cmd;
	int		i;
	char	*old_str;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->content)
		{
			i = 0;
			while (cmd->content[i])
			{
				old_str = expand_token_str(cmd->content[i], envp);
				free(cmd->content[i]);
				cmd->content[i] = old_str;
				old_str = ft_strip_quotes(cmd->content[i], 0);
				free(cmd->content[i]);
				cmd->content[i] = old_str;
				i++;
			}
		}
		cmd = cmd->next;
	}
}
