/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:14:23 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 16:40:11 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*ft_new_cmd(void)
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

/**
 * @brief parse tokens into an abstract syntax tree representing
 */
t_ast	*ft_parser(t_token **tokens)
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

static t_cmd	*parse_single_command(t_token **temp, int i)
{
	t_cmd	*cmd;
	int		arg_count;

	if ((*temp)->type == L_PAREN)
		return (parse_subshell(temp));
	cmd = ft_new_cmd();
	if (!cmd)
		return (NULL);
	arg_count = ft_count_args(*temp);
	cmd->content = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->content)
		return (NULL);
	while (*temp && (*temp)->type != PIPE && (*temp)->type != AND
		&& (*temp)->type != OR && (*temp)->type != R_PAREN)
	{
		if ((*temp)->type == WORD)
			cmd->content[i++] = ft_strdup((*temp)->content);
		else if ((*temp)->type == REDIRECT_IN || (*temp)->type == REDIRECT_OUT
			|| (*temp)->type == APPEND || (*temp)->type == HEREDOC)
		{
			if (!ft_redirection(cmd, temp))
				return (ft_free_cmd_list(cmd), NULL);
		}
		if (*temp)
			*temp = (*temp)->next;
	}
	return (cmd->content[i] = NULL, cmd);
}

static int	handle_pipe_token(t_token **tokens, t_cmd *list)
{
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type == PIPE
		|| (*tokens)->type == AND
		|| (*tokens)->type == OR
		|| (*tokens)->type == R_PAREN)
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `|'\n", 2);
		g_signal = 2;
		ft_free_cmd_list(list);
		return (0);
	}
	return (1);
}

t_cmd	*ft_parse_pipeline(t_token **tokens)
{
	t_cmd	*list;
	t_cmd	*current;
	t_cmd	*new_node;

	list = NULL;
	current = NULL;
	while (*tokens && (*tokens)->type != AND
		&& (*tokens)->type != OR
		&& (*tokens)->type != R_PAREN)
	{
		if ((*tokens)->type == PIPE)
		{
			if (!handle_pipe_token(tokens, list))
				return (NULL);
			continue ;
		}
		new_node = parse_single_command(tokens, 0);
		if (!new_node)
			return (ft_free_cmd_list(list), NULL);
		ft_add_cmd(&list, &current, new_node);
	}
	return (list);
}
