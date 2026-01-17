/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 22:24:46 by xin               #+#    #+#             */
/*   Updated: 2026/01/17 15:33:31 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_wildcard(t_redir *redir)
{
	char	**matches;
	int		i;

	matches = expand_wildcard(redir->file);
	if (!matches)
		return (0);
	if (matches[0] && matches[1])
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		i = 0;
		while (matches[i])
			free(matches[i++]);
		return (free(matches), 1);
	}
	else if (matches[0] && !matches[1])
	{
		free(redir->file);
		redir->file = ft_strdup(matches[0]);
	}
	i = 0;
	while (matches[i])
		free(matches[i++]);
	return (free(matches), 0);
}

static int	expand_redir_filename(t_redir *redir, t_env **env)
{
	char	*expanded;

	expanded = expand_token_str(redir->file, env);
	free(redir->file);
	redir->file = expanded;
	expanded = ft_strip_quotes(redir->file, 0);
	free(redir->file);
	redir->file = expanded;
	if (handle_wildcard(redir))
		return (1);
	return (0);
}

static int	expand_redirections(t_cmd *cmd, t_env **env)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			if (expand_redir_filename(redir, env))
				return (1);
		}
		redir = redir->next;
	}
	return (0);
}

int	ft_expand_pipeline(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->content)
			expand_command_args(cmd, env);
		if (expand_redirections(cmd, env))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
