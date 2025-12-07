/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:25:16 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 00:07:20 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	return (0);
}

static int	ft_check_token_syntax(t_token *tokens)
{
	t_token	*temp;

	temp = tokens;
	if (temp && temp->type == PIPE)
		return (syntax_error("|"));
	while (temp)
	{
		if (temp->type == PIPE)
		{
			if (temp->next == NULL || temp->next->type == PIPE)
				return (syntax_error("|"));
		}
		else if (temp->type != WORD)
		{
			if (temp->next == NULL)
				return (syntax_error("newline"));
			if (temp->next->type != WORD)
				return (syntax_error(temp->next->content));
		}
		temp = temp->next;
	}
	return (1);
}

static int	handle_operator(char *line, int i, t_token **t)
{
	if (line[i] == '|')
		return (add_token(t, create_token(ft_strdup("|"), PIPE)), i + 1);
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(t, create_token(ft_strdup("<<"), HEREDOC));
			return (i + 2);
		}
		add_token(t, create_token(ft_strdup("<"), REDIRECT_IN));
		return (i + 1);
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(t, create_token(ft_strdup(">>"), APPEND));
			return (i + 2);
		}
		add_token(t, create_token(ft_strdup(">"), REDIRECT_OUT));
		return (i + 1);
	}
	return (i);
}

static int	handle_word(char *line, int i, t_token **head)
{
	int		start;
	char	quote;
	char	*word;

	start = i;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"')
			&& (quote == 0 || quote == line[i]))
		{
			if (quote == 0)
				quote = line[i];
			else
				quote = 0;
		}
		else if (quote == 0 && ft_is_separator(line[i]))
			break ;
		i++;
	}
	if (quote != 0)
		return (ft_putstr_fd("minishell: syntax error: \
			unclosed quote\n", 2), -1);
	word = ft_substr(line, start, i - start);
	return (add_token(head, create_token(word, WORD)), i);
}

t_token	*ft_lexer(char *line)
{
	t_token	*list;
	int		i;

	list = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
		{
			i++;
			continue ;
		}
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			i = handle_operator(line, i, &list);
		else
			i = handle_word(line, i, &list);
	}
	if (!ft_check_token_syntax(list))
	{
		ft_free_tokens(&list);
		return (NULL);
	}
	return (list);
}
