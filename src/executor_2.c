/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:25:06 by xin               #+#    #+#             */
/*   Updated: 2025/12/09 00:11:28 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_builtin_redirect(t_cmd *cmd, int *saved_stdout, int *saved_stdin)
{
	int		fd;
	t_redir	*redir;

	*saved_stdout = -1;
	*saved_stdin = -1;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				return (-1);
			}
			if (*saved_stdin == -1)
				*saved_stdin = dup(STDIN_FILENO);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (redir->type == REDIR_APPEND)
				fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redir->file);
				return (-1);
			}
			if (*saved_stdout == -1)
				*saved_stdout = dup(STDOUT_FILENO);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}

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

int	ft_process_heredoc(t_cmd *cmd, t_env *env)
{
	t_cmd	*current;
	t_redir	*redir;

	current = cmd;
	while (current)
	{
		redir = current->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (ft_heredoc(redir, env) == -1)
				{
					perror("heredoc");
					return (-1);
				}
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}
