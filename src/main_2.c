/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:43:00 by meyu              #+#    #+#             */
/*   Updated: 2026/01/17 13:33:23 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_get_input(void)
{
	char	*line;
	char	*gnl_line;

	if (isatty(STDIN_FILENO))
		line = readline("minishell$ ");
	else
	{
		gnl_line = get_next_line(STDIN_FILENO);
		if (gnl_line)
		{
			line = ft_strtrim(gnl_line, "\n");
			free(gnl_line);
		}
		else
			line = NULL;
	}
	return (line);
}

int	has_unclosed_quote(char *str)
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
		i++;
	}
	return (quote);
}

char	*read_quote_line(void)
{
	char	*line;
	char	*gnl;

	if (isatty(STDIN_FILENO))
		return (readline("> "));
	gnl = get_next_line(STDIN_FILENO);
	if (!gnl)
		return (NULL);
	line = ft_strtrim(gnl, "\n");
	free(gnl);
	return (line);
}
