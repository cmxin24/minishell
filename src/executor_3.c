/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 18:44:04 by meyu              #+#    #+#             */
/*   Updated: 2025/12/21 22:57:13 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_input_redir(t_redir *redir, int *saved_stdin)
{
	int	fd;

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
	return (0);
}

static int	handle_output_redir(t_redir *redir, int *saved_stdout)
{
	int	fd;

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
	return (0);
}

int	ft_builtin_redirect(t_cmd *cmd, int *saved_stdout, int *saved_stdin)
{
	t_redir	*redir;
	int		ret;

	*saved_stdout = -1;
	*saved_stdin = -1;
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			ret = handle_input_redir(redir, saved_stdin);
			if (ret == -1)
				return (-1);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			ret = handle_output_redir(redir, saved_stdout);
			if (ret == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}
