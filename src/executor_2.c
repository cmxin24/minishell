/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:25:06 by xin               #+#    #+#             */
/*   Updated: 2025/12/16 13:11:52 by meyu             ###   ########.fr       */
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
				ft_putstr_fd("minishell: ", 2);
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
				ft_putstr_fd("minishell: ", 2);
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
	char	*expanded_delim;
	char	*stripped_delim;

	current = cmd;
	while (current)
	{
		redir = current->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (redir->heredoc_quoted == 0)
				{
					expanded_delim = expand_token_str(redir->file, &env);
					stripped_delim = ft_strip_quotes(expanded_delim, 0);
					free(expanded_delim);
				}
				else
				{
					stripped_delim = ft_strip_quotes(redir->file, 0);
				}
				free(redir->file);
				redir->file = stripped_delim;
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
