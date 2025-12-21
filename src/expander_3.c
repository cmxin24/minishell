/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 22:20:27 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 22:57:15 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	add_final_word(t_list **args, char *word)
{
	char	*final;

	final = ft_strip_quotes(word, 0);
	if (final)
		ft_lstadd_back(args, ft_lstnew(final));
}

static void	handle_wildcard_or_literal(t_list **args, char *token)
{
	char	**matches;
	int		i;

	matches = expand_wildcard(token);
	if (matches)
	{
		i = 0;
		while (matches[i])
		{
			add_final_word(args, matches[i]);
			free(matches[i]);
			i++;
		}
		free(matches);
	}
	else
		add_final_word(args, token);
}

static void	handle_split_tokens(t_list **args, char **split)
{
	int	j;

	j = 0;
	while (split[j])
	{
		handle_wildcard_or_literal(args, split[j]);
		free(split[j]);
		j++;
	}
	free(split);
}

static void	expand_one_arg(t_list **args, char *arg, t_env **env)
{
	char	*expanded;
	char	**split;

	expanded = expand_token_str(arg, env);
	if (!expanded)
		return ;
	split = ft_split_unquoted(expanded);
	free(expanded);
	if (!split)
		return ;
	handle_split_tokens(args, split);
}

void	expand_command_args(t_cmd *cmd, t_env **env)
{
	t_list	*new_args;
	int		i;

	new_args = NULL;
	i = 0;
	while (cmd->content[i])
	{
		expand_one_arg(&new_args, cmd->content[i], env);
		i++;
	}
	i = 0;
	while (cmd->content[i])
		free(cmd->content[i++]);
	free(cmd->content);
	cmd->content = list_to_array(new_args);
	free_list_nodes(new_args);
}
