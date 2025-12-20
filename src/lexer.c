/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:25:16 by xin               #+#    #+#             */
/*   Updated: 2025/12/20 21:28:13 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_word(char *line, int i, t_token **list)
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
	return (add_token(list, create_token(word, WORD)), i);
}

static t_token	*lexer_tokenize(char *line)
{
	t_token	*list;
	int		i;

	list = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
		{
			i++;
			continue ;
		}
		if (line[i] == '|' || line[i] == '<' || line[i] == '>'
			|| line[i] == '&' || line[i] == '(' || line[i] == ')')
			i = handle_operator(line, i, &list);
		else
			i = handle_word(line, i, &list);
	}
	return (list);
}

static int	is_heredoc_delimiter(char *line, char *delim)
{
	char	*trimmed;
	int		match;

	if (isatty(STDIN_FILENO))
		return (ft_strcmp(line, delim) == 0);
	trimmed = ft_strtrim(line, "\n");
	match = (ft_strcmp(trimmed, delim) == 0);
	free(trimmed);
	return (match);
}

static void	skip_one_heredoc(t_token *t)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (is_heredoc_delimiter(line, t->next->content))
		{
			free(line);
			break ;
		}
		free(line);
	}
}

/**
 * @brief convert the input line into a list of tokens
 * @note
 * 1. skip whitespace
 * 2. identify operators and create corresponding tokens
 * 3. identify words and create corresponding tokens
 * 4. check for syntax errors in the token list
 */
t_token	*ft_lexer(char *line)
{
	t_token	*list;
	t_token	*temp;

	list = lexer_tokenize(line);
	if (!list)
		return (NULL);
	if (!ft_check_token_syntax(list))
	{
		temp = list;
		while (temp)
		{
			if (temp->type == HEREDOC && temp->next
				&& temp->next->type == WORD)
				skip_one_heredoc(temp);
			temp = temp->next;
		}
		ft_free_tokens(&list);
		return (NULL);
	}
	return (list);
}
