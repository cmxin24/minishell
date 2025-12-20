/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 21:30:05 by xin               #+#    #+#             */
/*   Updated: 2025/12/20 21:30:35 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_lparen(t_token *t, int *parens)
{
	(*parens)++;
	if (t->next && (t->next->type == PIPE
			|| t->next->type == AND
			|| t->next->type == OR
			|| t->next->type == R_PAREN))
		return (syntax_error(t->next->content));
	if (t->next == NULL)
		return (syntax_error("newline"));
	return (1);
}

int	check_rparen(t_token *t, int *parens)
{
	(*parens)--;
	if (*parens < 0)
		return (syntax_error(")"));
	if (t->next && (t->next->type == WORD
			|| t->next->type == L_PAREN))
		return (syntax_error(t->next->content));
	return (1);
}

int	check_operator(t_token *t)
{
	if (t->next == NULL)
		return (syntax_error("newline"));
	if (t->next->type == PIPE || t->next->type == AND
		|| t->next->type == OR || t->next->type == R_PAREN)
		return (syntax_error(t->next->content));
	return (1);
}

int	check_redirect(t_token *t)
{
	if (t->next == NULL)
		return (syntax_error("newline"));
	if (t->next->type != WORD)
		return (syntax_error(t->next->content));
	return (1);
}

int	check_word(t_token *t)
{
	if (t->next && t->next->type == L_PAREN)
		return (syntax_error(t->next->content));
	return (1);
}
