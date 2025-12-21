/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 10:00:00 by copilot           #+#    #+#             */
/*   Updated: 2025/12/22 00:35:44 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>

static int	pattern_starts_with_dot(char *pattern)
{
	int		i;
	char	quote;

	i = 0;
	while (pattern[i])
	{
		if (pattern[i] == '.')
			return (1);
		if (pattern[i] == '\'' || pattern[i] == '\"')
		{
			quote = pattern[i];
			if (pattern[i + 1] == quote)
			{
				i += 2;
				continue ;
			}
			if (pattern[i + 1] == '.')
				return (1);
			return (0);
		}
		return (0);
	}
	return (0);
}

static int	count_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && !pattern_starts_with_dot(pattern))
		{
			entry = readdir(dir);
			continue ;
		}
		if (match(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static void	fill_matches(char *pattern, char **matches)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	dir = opendir(".");
	if (!dir)
		return ;
	i = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && !pattern_starts_with_dot(pattern))
		{
			entry = readdir(dir);
			continue ;
		}
		if (match(pattern, entry->d_name))
			matches[i++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	matches[i] = NULL;
	closedir(dir);
}

static void	sort_matches(char **matches)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (matches[i])
	{
		j = i + 1;
		while (matches[j])
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				temp = matches[i];
				matches[i] = matches[j];
				matches[j] = temp;
			}
			j++;
		}
		i++;
	}
}

char	**expand_wildcard(char *pattern)
{
	char	**matches;
	int		count;

	if (!has_wildcard(pattern))
		return (NULL);
	count = count_matches(pattern);
	if (count == 0)
		return (NULL);
	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	fill_matches(pattern, matches);
	sort_matches(matches);
	return (matches);
}
