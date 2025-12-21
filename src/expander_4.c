/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 22:24:46 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 23:00:05 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_list_nodes(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

char	**list_to_array(t_list *list)
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

static void	expand_redir_filename(t_redir *redir, t_env **env)
{
	char	*expanded;
	char	**matches;
	int		i;

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
		i = 0;
		while (matches[i])
			free(matches[i++]);
		free(matches);
	}
	expanded = ft_strip_quotes(redir->file, 0);
	free(redir->file);
	redir->file = expanded;
}

static void	expand_redirections(t_cmd *cmd, t_env **env)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
			expand_redir_filename(redir, env);
		redir = redir->next;
	}
}

void	ft_expand_pipeline(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->content)
			expand_command_args(cmd, env);
		expand_redirections(cmd, env);
		cmd = cmd->next;
	}
}
