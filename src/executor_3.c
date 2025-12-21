/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:44:04 by meyu              #+#    #+#             */
/*   Updated: 2025/12/21 18:45:21 by meyu             ###   ########.fr       */
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
