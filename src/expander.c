/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:47:12 by xin               #+#    #+#             */
/*   Updated: 2025/12/12 16:36:04 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_len_without_quotes(char *str)
{
	int		len;
	int		i;
	char	quote;

	len = 0;
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '\\' && quote != '\'')
		{
			if (quote == '\"')
			{
				if (str[i + 1] == '$' || str[i + 1] == '\"'
					|| str[i + 1] == '\\')
				{
					i++;
					if (str[i])
						len++;
				}
				else
					len++;
			}
			else
			{
				i++;
				if (str[i])
					len++;
			}
		}
		else if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			len++;
		i++;
	}
	return (len);
}

char	*ft_strip_quotes(char *str, int len)
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
		if (str[i] == '\\' && quote != '\'')
		{
			if (quote == '\"')
			{
				if (str[i + 1] == '$' || str[i + 1] == '\"'
					|| str[i + 1] == '\\')
				{
					i++;
					if (str[i])
						new_str[j++] = str[i];
				}
				else
					new_str[j++] = str[i];
			}
			else
			{
				i++;
				if (str[i])
					new_str[j++] = str[i];
			}
		}
		else if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			new_str[j++] = str[i];
		i++;
	}
	return (new_str[j] = '\0', new_str);
}

static void	remove_empty_arg(t_cmd *cmd, int index)
{
	int	i;

	free(cmd->content[index]);
	i = index;
	while (cmd->content[i + 1])
	{
		cmd->content[i] = cmd->content[i + 1];
		i++;
	}
	cmd->content[i] = NULL;
}

void	ft_expander(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*cmd;
	int		i;
	char	*old_str;
	t_redir	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->content)
		{
			i = 0;
			while (cmd->content[i])
			{
				old_str = expand_token_str(cmd->content[i], env);
				free(cmd->content[i]);
				cmd->content[i] = old_str;
				if (cmd->content[i][0] == '\0')
				{
					remove_empty_arg(cmd, i);
					continue ;
				}
				old_str = ft_strip_quotes(cmd->content[i], 0);
				free(cmd->content[i]);
				cmd->content[i] = old_str;
				i++;
			}
		}
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type != REDIR_HEREDOC)
			{
				old_str = expand_token_str(redir->file, env);
				free(redir->file);
				redir->file = old_str;
				old_str = ft_strip_quotes(redir->file, 0);
				free(redir->file);
				redir->file = old_str;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}
