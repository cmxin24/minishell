/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:14:23 by xin               #+#    #+#             */
/*   Updated: 2025/12/19 12:21:20 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*parse_or(t_token **tokens);

static t_cmd	*ft_new_cmd(void)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->content = NULL;
	new->redirs = NULL;
	new->next = NULL;
	new->subshell = NULL;
	return (new);
}

static int	ft_count_args(t_token *token)
{
	int	i;

	i = 0;
	while (token && token->type != PIPE && token->type != AND && token->type != OR && token->type != R_PAREN)
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

static t_cmd	*parse_subshell(t_token **tokens)
{
	t_cmd	*cmd;

	cmd = ft_new_cmd();
	if (!cmd)
		return (NULL);
	*tokens = (*tokens)->next;
	cmd->subshell = parse_or(tokens);
	if (*tokens && (*tokens)->type == R_PAREN)
		*tokens = (*tokens)->next;
	else
		ft_putstr_fd("minishell: syntax error: expected ')'\n", 2);
	while (*tokens && ((*tokens)->type == REDIRECT_IN || (*tokens)->type == REDIRECT_OUT
			|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC))
	{
		ft_redirection(cmd, tokens);
		if (*tokens)
			*tokens = (*tokens)->next;
	}
	return (cmd);
}

static t_cmd	*parse_single_command(t_token **temp)
{
	t_cmd	*cmd;
	int		arg_count;
	int		i;

	if ((*temp)->type == L_PAREN)
		return (parse_subshell(temp));
	cmd = ft_new_cmd();
	if (!cmd)
		return (NULL);
	arg_count = ft_count_args(*temp);
	cmd->content = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->content)
		return (NULL);
	i = 0;
	while (*temp && (*temp)->type != PIPE && (*temp)->type != AND && (*temp)->type != OR && (*temp)->type != R_PAREN)
	{
		if ((*temp)->type == WORD)
			cmd->content[i++] = ft_strdup((*temp)->content);
		else if ((*temp)->type == REDIRECT_IN || (*temp)->type == REDIRECT_OUT
			|| (*temp)->type == APPEND || (*temp)->type == HEREDOC)
			ft_redirection(cmd, temp);
		if (*temp)
			*temp = (*temp)->next;
	}
	cmd->content[i] = NULL;
	return (cmd);
}

static void	ft_add_cmd(t_cmd **list, t_cmd **current, t_cmd *new_cmd)
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

t_cmd	*ft_parse_pipeline(t_token **tokens)
{
	t_cmd	*list;
	t_cmd	*current;
	t_cmd	*new_node;

	list = NULL;
	current = NULL;
	while (*tokens && (*tokens)->type != AND && (*tokens)->type != OR && (*tokens)->type != R_PAREN)
	{
		if ((*tokens)->type == PIPE)
		{
			*tokens = (*tokens)->next;
			if (!*tokens || (*tokens)->type == PIPE || (*tokens)->type == AND || (*tokens)->type == OR || (*tokens)->type == R_PAREN)
				break ;
			continue ;
		}
		new_node = parse_single_command(tokens);
		if (!new_node)
		{
			ft_free_cmd_list(list);
			return (NULL);
		}
		ft_add_cmd(&list, &current, new_node);
	}
	return (list);
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

t_ast	*parse_or(t_token **tokens)
{
	t_ast			*left;
	t_ast			*right;
	t_ast			*node;
	t_type_of_token	type;

	left = parse_primary(tokens);
	while (*tokens && ((*tokens)->type == AND || (*tokens)->type == OR))
	{
		type = (*tokens)->type;
		*tokens = (*tokens)->next;
		right = parse_primary(tokens);
		node = malloc(sizeof(t_ast));
		if (type == AND)
			node->type = AST_AND;
		else
			node->type = AST_OR;
		node->left = left;
		node->right = right;
		node->pipeline = NULL;
		left = node;
	}
	return (left);
}

t_ast	*ft_parser(t_token *tokens)
{
	return (parse_or(&tokens));
}
