/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:47:12 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 17:02:51 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_len_without_quotes(char *str)
{
	int		len;
	int		i;
	char	quote;

	len = 0;
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '\\' && quote != '\'')
		{
			if (quote == '\"')
			{
				if (str[i + 1] == '$' || str[i + 1] == '\"'
					|| str[i + 1] == '\\')
				{
					i++;
					if (str[i])
						len++;
				}
				else
					len++;
			}
			else
			{
				i++;
				if (str[i])
					len++;
			}
		}
		else if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			len++;
		i++;
	}
	return (len);
}

char	*ft_strip_quotes(char *str, int len)
{
	char	*new_str;
	int		i;
	int		j;
	char	quote;

	if (!str)
		return (NULL);
	len = ft_len_without_quotes(str);
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '\\' && quote != '\'')
		{
			if (quote == '\"')
			{
				if (str[i + 1] == '$' || str[i + 1] == '\"'
					|| str[i + 1] == '\\')
				{
					i++;
					if (str[i])
						new_str[j++] = str[i];
				}
				else
					new_str[j++] = str[i];
			}
			else
			{
				i++;
				if (str[i])
					new_str[j++] = str[i];
			}
		}
		else if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			new_str[j++] = str[i];
		i++;
	}
	return (new_str[j] = '\0', new_str);
}

static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	count_words(char *str)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
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

static char	**ft_split_unquoted(char *str)
{
	int		count;
	char	**result;
	int		i;
	int		str_index;

	if (!str)
		return (NULL);
	count = count_words(str);
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

static char	**list_to_array(t_list *list)
{
	int		len;
	char	**array;
	int		i;
	t_list	*tmp;

	len = ft_lstsize(list);
	array = malloc(sizeof(char *) * (len + 1));
	if (!array)
		return (NULL);
	i = 0;
	tmp = list;
	while (tmp)
	{
		array[i++] = tmp->content;
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}

static void	free_list_nodes(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

void	ft_expand_pipeline(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*cmd;
	int		i;
	char	*expanded;
	t_redir	*redir;
	char	**matches;
	t_list	*new_args;
	char	**split;
	int		j;
	int		k;
	char	*final;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->content)
		{
			new_args = NULL;
			i = 0;
			while (cmd->content[i])
			{
				expanded = expand_token_str(cmd->content[i], env);
				split = ft_split_unquoted(expanded);
				free(expanded);
				if (!split)
				{
					i++;
					continue ;
				}
				j = 0;
				while (split[j])
				{
					matches = expand_wildcard(split[j]);
					if (matches)
					{
						k = 0;
						while (matches[k])
						{
							final = ft_strip_quotes(matches[k], 0);
							ft_lstadd_back(&new_args, ft_lstnew(final));
							free(matches[k]);
							k++;
						}
						free(matches);
					}
					else
					{
						final = ft_strip_quotes(split[j], 0);
						if (final)
							ft_lstadd_back(&new_args, ft_lstnew(final));
					}
					free(split[j]);
					j++;
				}
				free(split);
				i++;
			}
			i = 0;
			while (cmd->content[i])
				free(cmd->content[i++]);
			free(cmd->content);
			cmd->content = list_to_array(new_args);
			free_list_nodes(new_args);
		}
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type != REDIR_HEREDOC)
			{
				expanded = expand_token_str(redir->file, env);
				free(redir->file);
				redir->file = expanded;
				matches = expand_wildcard(redir->file);
				if (matches)
				{
					if (matches[0] && !matches[1])
					{
						free(redir->file);
						redir->file = ft_strdup(matches[0]);
					}
					k = 0;
					while (matches[k])
						free(matches[k++]);
					free(matches);
				}
				expanded = ft_strip_quotes(redir->file, 0);
				free(redir->file);
				redir->file = expanded;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
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
