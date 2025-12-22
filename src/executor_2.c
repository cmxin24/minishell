/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:25:06 by xin               #+#    #+#             */
/*   Updated: 2025/12/22 16:10:33 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_restore_io(int saved_stdout, int saved_stdin)
{
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
}

int	ft_process_heredoc(t_ast *ast, t_env *env);

static char	*process_heredoc_delimiter(t_redir *redir, t_env *env)
{
	(void)env;
	return (ft_strip_quotes(redir->file, 0));
}

static int	handle_heredoc_redir(t_redir *redir, t_env *env)
{
	char	*new_delim;

	new_delim = process_heredoc_delimiter(redir, env);
	free(redir->file);
	redir->file = new_delim;
	if (ft_heredoc(redir, env) == -1)
	{
		if (g_signal != 130)
			perror("heredoc");
		return (-1);
	}
	return (0);
}

static int	process_heredoc_pipeline(t_cmd *cmd, t_env *env)
{
	t_cmd	*current;
	t_redir	*redir;

	current = cmd;
	while (current)
	{
		if (current->subshell)
		{
			if (ft_process_heredoc(current->subshell, env) == -1)
				return (-1);
		}
		redir = current->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (handle_heredoc_redir(redir, env) == -1)
					return (-1);
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}

int	ft_process_heredoc(t_ast *ast, t_env *env)
{
	if (!ast)
		return (0);
	if (ast->type == AST_PIPELINE)
		return (process_heredoc_pipeline(ast->pipeline, env));
	else
	{
		if (ft_process_heredoc(ast->left, env) == -1)
			return (-1);
		if (ft_process_heredoc(ast->right, env) == -1)
			return (-1);
	}
	return (0);
}
