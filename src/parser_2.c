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

static void	ft_add_redir(t_cmd *cmd, t_redir *new)
{
	t_redir	*tmp;

	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	ft_redirection(t_cmd *cmd, t_token **token)
{
	t_type_of_token	type;
	t_redir			*new_redir;

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
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->next = NULL;
	new_redir->heredoc_quoted = 0;
	if (type == HEREDOC)
	{
		new_redir->type = REDIR_HEREDOC;
		if (if_quotes((*token)->content))
			new_redir->heredoc_quoted = 1;
		new_redir->file = strip_heredoc_quotes((*token)->content);
	}
	else if (type == REDIRECT_IN)
	{
		new_redir->type = REDIR_IN;
		new_redir->file = ft_strdup((*token)->content);
	}
	else if (type == REDIRECT_OUT || type == APPEND)
	{
		if (type == APPEND)
			new_redir->type = REDIR_APPEND;
		else
			new_redir->type = REDIR_OUT;
		new_redir->file = ft_strdup((*token)->content);
	}
	ft_add_redir(cmd, new_redir);
}
