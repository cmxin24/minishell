/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:04:08 by xin               #+#    #+#             */
/*   Updated: 2025/12/08 19:30:25 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	if_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

static char	*strip_heredoc_quotes(char *str)
{
	int		len;
	char	*new_str;

	if (!if_quotes(str))
		return (ft_strdup(str));
	len = ft_len_without_quotes(str);
	new_str = ft_strip_quotes(str, len);
	return (new_str);
}

void	ft_redirection(t_cmd *cmd, t_token **token)
{
	t_type_of_token	type;

	type = (*token)->type;
	*token = (*token)->next;
	if (!*token || (*token)->type != WORD)
	{
		if (!*token)
			syntax_error("newline");
		else
			syntax_error((*token)->content);
		return ;
	}
	if (type == HEREDOC)
	{
		if (cmd->redirect_in)
			free(cmd->redirect_in);
		if (if_quotes((*token)->content))
			cmd->heredoc_quoted = 1;
		else
			cmd->heredoc_quoted = 0;
		cmd->redirect_in = strip_heredoc_quotes((*token)->content);
		cmd->is_heredoc = 1;
	}
	else if (type == REDIRECT_IN)
	{
		if (cmd->redirect_in)
			free(cmd->redirect_in);
		cmd->redirect_in = ft_strdup((*token)->content);
		cmd->is_heredoc = 0;
	}
	else if (type == REDIRECT_OUT || type == APPEND)
	{
		if (cmd->redirect_out)
			free(cmd->redirect_out);
		cmd->redirect_out = ft_strdup((*token)->content);
		if (type == APPEND)
			cmd->is_append = 1;
		else
			cmd->is_append = 0;
	}
}
