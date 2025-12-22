/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:04:08 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 16:35:47 by meyu             ###   ########.fr       */
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

static int	validate_redir_token(t_token *token)
{
	if (!token || token->type != WORD)
	{
		if (!token)
			syntax_error("newline");
		else
			syntax_error(token->content);
		return (0);
	}
	return (1);
}

static t_redir	*create_redir(void)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->next = NULL;
	new_redir->heredoc_quoted = 0;
	new_redir->file = NULL;
	return (new_redir);
}

static void	set_redir_info(t_redir *redir, t_type_of_token type, t_token *token)
{
	if (type == HEREDOC)
	{
		redir->type = REDIR_HEREDOC;
		if (if_quotes(token->content))
			redir->heredoc_quoted = 1;
		redir->file = ft_strdup(token->content);
	}
	else if (type == REDIRECT_IN)
	{
		redir->type = REDIR_IN;
		redir->file = ft_strdup(token->content);
	}
	else if (type == REDIRECT_OUT || type == APPEND)
	{
		if (type == APPEND)
			redir->type = REDIR_APPEND;
		else
			redir->type = REDIR_OUT;
		redir->file = ft_strdup(token->content);
	}
}

int	ft_redirection(t_cmd *cmd, t_token **token)
{
	t_type_of_token	type;
	t_redir			*new_redir;
	t_redir			*tmp;

	type = (*token)->type;
	*token = (*token)->next;
	if (!validate_redir_token(*token))
		return (0);
	new_redir = create_redir();
	if (!new_redir)
		return (0);
	set_redir_info(new_redir, type, *token);
	if (!cmd->redirs)
		cmd->redirs = new_redir;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_redir;
	}
	return (1);
}
