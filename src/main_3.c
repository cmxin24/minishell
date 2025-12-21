/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:02:31 by meyu              #+#    #+#             */
/*   Updated: 2025/12/21 18:10:11 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_lines(char *line, int i, int in_word)
{
	int		count;
	char	quote;

	count = 0;
	quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		if (line[i] != '\n' || quote)
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (count);
}

char	*get_next_line_segment(char *line, int *index)
{
	int		start;
	int		i;
	char	quote;

	i = *index;
	quote = 0;
	while (line[i] == '\n')
		i++;
	start = i;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		if (line[i] == '\n' && !quote)
			break ;
		i++;
	}
	*index = i;
	return (ft_substr(line, start, i - start));
}

char	**ft_split_lines_safe(char *line)
{
	int		count;
	char	**result;
	int		i;
	int		line_idx;

	count = count_lines(line, 0, 0);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	line_idx = 0;
	while (i < count)
	{
		result[i] = get_next_line_segment(line, &line_idx);
		i++;
	}
	result[i] = NULL;
	return (result);
}
