/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:38:41 by xin               #+#    #+#             */
/*   Updated: 2025/12/20 21:28:21 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '|'
		|| c == '<' || c == '>' || c == '&' || c == '(' || c == ')');
}

/**
 * @brief print syntax error message for unexpected token
 * @note
 * 258 is the bash syntax error code
 */
int	syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	g_signal = 258;
	return (0);
}

int	handle_operator(char *line, int i, t_token **t)
{
	if (line[i] == '|')
		return (handle_pipe(line, i, t));
	if (line[i] == '&')
		return (handle_and(line, i, t));
	if (line[i] == '<')
		return (handle_redirect_in(line, i, t));
	if (line[i] == '>')
		return (handle_redirect_out(line, i, t));
	if (line[i] == '(' || line[i] == ')')
		return (handle_paren(line[i], i, t));
	return (i);
}
