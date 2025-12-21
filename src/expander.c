/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:47:12 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 22:59:51 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	count_words(char *str, int i)
{
	int		count;
	char	quote;

	count = 0;
	quote = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i])
			count++;
		while (str[i])
		{
			if ((str[i] == '\'' || str[i] == '\"') && !quote)
				quote = str[i];
			else if (str[i] == quote)
				quote = 0;
			if (ft_isspace(str[i]) && !quote)
				break ;
			i++;
		}
	}
	return (count);
}

static char	*get_next_word(char *str, int *index)
{
	int		i;
	int		start;
	char	quote;

	i = *index;
	quote = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	start = i;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && !quote)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		if (ft_isspace(str[i]) && !quote)
			break ;
		i++;
	}
	*index = i;
	return (ft_substr(str, start, i - start));
}

char	**ft_split_unquoted(char *str)
{
	int		count;
	char	**result;
	int		i;
	int		str_index;

	if (!str)
		return (NULL);
	count = count_words(str, 0);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	str_index = 0;
	while (i < count)
	{
		result[i] = get_next_word(str, &str_index);
		i++;
	}
	result[i] = NULL;
	return (result);
}

void	ft_expander(t_ast *ast, t_env **env)
{
	if (!ast)
		return ;
	if (ast->type == AST_PIPELINE)
		ft_expand_pipeline(ast->pipeline, env);
	else
	{
		ft_expander(ast->left, env);
		ft_expander(ast->right, env);
	}
}
