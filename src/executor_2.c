/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:25:06 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 19:43:23 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_builtin_redirect(t_cmd *cmd, int *saved_stdout)
{
	int	fd;
	
	*saved_stdout = -1;
	if (cmd->redirect_out)
	{
		if (cmd->is_append)
			fd = open(cmd->redirect_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->redirect_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(cmd->redirect_out);
			return (-1);
		}
		*saved_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	
	return (0);
}
void	ft_restore_stdout(int saved_stdout)
{
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
