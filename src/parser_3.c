/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:29:35 by meyu              #+#    #+#             */
/*   Updated: 2025/12/22 16:40:16 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*parse_subshell(t_token **tokens)
{
	t_cmd	*cmd;

	cmd = ft_new_cmd();
	if (!cmd)
		return (NULL);
	*tokens = (*tokens)->next;
	cmd->subshell = ft_parser(tokens);
	if (*tokens && (*tokens)->type == R_PAREN)
		*tokens = (*tokens)->next;
	else
	{
		ft_putstr_fd("minishell: syntax error: expected ')'\n", 2);
		g_signal = 2;
		return (ft_free_cmd_list(cmd), NULL);
	}
	while (*tokens && ((*tokens)->type == REDIRECT_IN
			|| (*tokens)->type == REDIRECT_OUT
			|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC))
	{
		if (!ft_redirection(cmd, tokens))
			return (ft_free_cmd_list(cmd), NULL);
		if (*tokens)
			*tokens = (*tokens)->next;
	}
	return (cmd);
}

void	ft_add_cmd(t_cmd **list, t_cmd **current, t_cmd *new_cmd)
{
	if (!*list)
	{
		*list = new_cmd;
		*current = new_cmd;
	}
	else
	{
		(*current)->next = new_cmd;
		*current = (*current)->next;
	}
}

int	ft_count_args(t_token *token)
{
	int	i;

	i = 0;
	while (token && token->type != PIPE && token->type
		!= AND && token->type != OR && token->type != R_PAREN)
	{
		if (token->type == REDIRECT_IN || token->type == REDIRECT_OUT
			|| token->type == APPEND || token->type == HEREDOC)
		{
			if (token->next)
				token = token->next;
		}
		else if (token->type == WORD)
			i++;
		token = token->next;
	}
	return (i);
}

t_ast	*parse_primary(t_token **tokens)
{
	t_cmd	*pipeline;
	t_ast	*node;

	pipeline = ft_parse_pipeline(tokens);
	if (!pipeline)
		return (NULL);
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = AST_PIPELINE;
	node->pipeline = pipeline;
	node->left = NULL;
	node->right = NULL;
	return (node);
}
