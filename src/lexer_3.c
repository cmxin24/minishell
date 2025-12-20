/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:52:03 by xin               #+#    #+#             */
/*   Updated: 2025/12/20 21:31:31 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_start_token(t_token *t)
{
	if (t && (t->type == PIPE || t->type == AND
			|| t->type == OR || t->type == R_PAREN))
		return (syntax_error(t->content));
	return (1);
}

static int	ft_check_token_loop(t_token *temp, int parens)
{
	while (temp)
	{
		if (temp->type == L_PAREN && !check_lparen(temp, &parens))
			return (0);
		else if (temp->type == R_PAREN && !check_rparen(temp, &parens))
			return (0);
		else if ((temp->type == PIPE || temp->type == AND
				|| temp->type == OR) && !check_operator(temp))
			return (0);
		else if ((temp->type == REDIRECT_IN || temp->type == REDIRECT_OUT
				|| temp->type == APPEND || temp->type == HEREDOC)
			&& !check_redirect(temp))
			return (0);
		else if (temp->type == WORD && !check_word(temp))
			return (0);
		temp = temp->next;
	}
	return (1);
}

int	ft_check_token_syntax(t_token *tokens)
{
	t_token	*temp;
	int		parens;

	temp = tokens;
	parens = 0;
	if (!check_start_token(temp))
		return (0);
	if (!ft_check_token_loop(temp, parens))
		return (0);
	if (parens != 0)
		return (syntax_error("newline"));
	return (1);
}
