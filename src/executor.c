/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:38:25 by xin               #+#    #+#             */
/*   Updated: 2026/01/17 15:29:17 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	execute_single_builtin(t_cmd *cmd, t_env **env, int is_subshell)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = -1;
	saved_stdin = -1;
	if (!cmd->content || !cmd->content[0])
		return (0);
	if (!is_builtin(cmd->content[0]))
		return (0);
	if (ft_strcmp(cmd->content[0], "env") == 0 && cmd->content[1])
		return (0);
	if (ft_builtin_redirect(cmd, &saved_stdout, &saved_stdin) == 0)
	{
		g_signal = exec_builtin(cmd->content, env, is_subshell);
		ft_restore_io(saved_stdout, saved_stdin);
		return (1);
	}
	ft_restore_io(saved_stdout, saved_stdin);
	g_signal = 1;
	return (1);
}

pid_t	fork_pipeline(t_cmd *cmd, t_env **env, int fd_in, int pipe_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		child_process(cmd, env, pipe_fd, fd_in);
	return (pid);
}

static void	setup_pipeline_fds(t_cmd *current, int *fd_in, int pipe_fd[2])
{
	if (current->next)
	{
		close(pipe_fd[1]);
		if (*fd_in != 0)
			close(*fd_in);
		*fd_in = pipe_fd[0];
	}
	else if (*fd_in != 0)
		close(*fd_in);
}

void	execute_pipeline(t_cmd *cmd_list, t_env **env, int is_subshell)
{
	t_cmd	*current;
	int		fd_in;
	int		pipe_fd[2];
	pid_t	pid;

	current = cmd_list;
	fd_in = 0;
	if (!current->next && execute_single_builtin(current, env, is_subshell))
		return ;
	while (current)
	{
		if (current->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return ;
		}
		pid = fork_pipeline(current, env, fd_in, pipe_fd);
		setup_pipeline_fds(current, &fd_in, pipe_fd);
		current = current->next;
	}
	ft_wait_for_children(pid);
}

void	ft_executor(t_ast *ast, t_env **env, int is_subshell)
{
	if (!ast)
		return ;
	if (ast->type == AST_PIPELINE)
	{
		if (ft_expand_pipeline(ast->pipeline, env))
		{
			g_signal = 1;
			return ;
		}
		execute_pipeline(ast->pipeline, env, is_subshell);
	}
	else if (ast->type == AST_AND)
	{
		ft_executor(ast->left, env, is_subshell);
		if (g_signal == 0)
			ft_executor(ast->right, env, is_subshell);
	}
	else if (ast->type == AST_OR)
	{
		ft_executor(ast->left, env, is_subshell);
		if (g_signal != 0)
			ft_executor(ast->right, env, is_subshell);
	}
}
