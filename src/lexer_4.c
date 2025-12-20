/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 21:24:41 by xin               #+#    #+#             */
/*   Updated: 2025/12/20 21:25:32 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_pipe(char *line, int i, t_token **t)
{
	if (line[i + 1] == '|')
	{
		add_token(t, create_token(ft_strdup("||"), OR));
		return (i + 2);
	}
	add_token(t, create_token(ft_strdup("|"), PIPE));
	return (i + 1);
}

int	handle_and(char *line, int i, t_token **t)
{
	if (line[i + 1] == '&')
	{
		add_token(t, create_token(ft_strdup("&&"), AND));
		return (i + 2);
	}
	add_token(t, create_token(ft_strdup("&"), WORD));
	return (i + 1);
}

int	handle_redirect_in(char *line, int i, t_token **t)
{
	if (line[i + 1] == '<')
	{
		add_token(t, create_token(ft_strdup("<<"), HEREDOC));
		return (i + 2);
	}
	add_token(t, create_token(ft_strdup("<"), REDIRECT_IN));
	return (i + 1);
}

int	handle_redirect_out(char *line, int i, t_token **t)
{
	if (line[i + 1] == '>')
	{
		add_token(t, create_token(ft_strdup(">>"), APPEND));
		return (i + 2);
	}
	add_token(t, create_token(ft_strdup(">"), REDIRECT_OUT));
	return (i + 1);
}

int	handle_paren(char c, int i, t_token **t)
{
	if (c == '(')
		add_token(t, create_token(ft_strdup("("), L_PAREN));
	else
		add_token(t, create_token(ft_strdup(")"), R_PAREN));
	return (i + 1);
}
