/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:33:13 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 00:35:34 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	match_star(char *pattern, char *string)
{
	while (*pattern == '*')
		pattern++;
	if (!*pattern)
		return (1);
	while (*string)
	{
		if (match(pattern, string))
			return (1);
		string++;
	}
	return (0);
}

static int	match_quoted(char **pattern, char **string)
{
	char	quote;

	quote = **pattern;
	(*pattern)++;
	while (**pattern && **pattern != quote)
	{
		if (**pattern != **string)
			return (0);
		(*pattern)++;
		(*string)++;
	}
	if (**pattern == quote)
		(*pattern)++;
	return (1);
}

int	match(char *pattern, char *string)
{
	while (*pattern && *string)
	{
		if (*pattern == '*')
			return (match_star(pattern, string));
		if (*pattern == '\'' || *pattern == '\"')
		{
			if (!match_quoted(&pattern, &string))
				return (0);
		}
		else
		{
			if (*pattern != *string)
				return (0);
			pattern++;
			string++;
		}
	}
	if (*pattern == '*')
		while (*pattern == '*')
			pattern++;
	return (!*pattern && !*string);
}

int	has_wildcard(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else if (str[i] == '*' && quote == 0)
			return (1);
		i++;
	}
	return (0);
}
