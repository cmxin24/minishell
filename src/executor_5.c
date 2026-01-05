/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:46:06 by xin               #+#    #+#             */
/*   Updated: 2026/01/05 21:03:40 by nschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_internal_cmd(t_cmd *cmd, t_env **env, char **env_array)
{
	if (cmd->subshell)
	{
		ft_executor(cmd->subshell, env, 1);
		ft_free_array(env_array);
		exit(g_signal);
	}
	if (cmd->content == NULL || cmd->content[0] == NULL)
	{
		ft_free_array(env_array);
		exit(0);
	}
	if (is_builtin(cmd->content[0]) && !(ft_strcmp(cmd->content[0], "env")
			== 0 && cmd->content[1]))
	{
		ft_free_array(env_array);
		exit(exec_builtin(cmd->content, env, 1));
	}
}

void	child_execute_cmd(t_cmd *cmd, t_env **env, char **env_array)
{
	char	*path;

	handle_internal_cmd(cmd, env, env_array);
	path = find_command_path(cmd->content[0], env_array, 0, NULL);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->content[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_array(env_array);
		exit(127);
	}
	execve(path, cmd->content, env_array);
	ft_putstr_fd("minishell: ", 2);
	perror(cmd->content[0]);
	free(path);
	ft_free_array(env_array);
	if (errno == ENOENT)
		exit(127);
	if (errno == EACCES || errno == EISDIR)
		exit(126);
	exit(1);
}

static void	init_child_signals(void)
{
	struct sigaction	sa;
	sigset_t			set;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_DFL;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
	sigemptyset(&set);
	sigprocmask(SIG_SETMASK, &set, NULL);
}

static int	child_handle_redirs(t_cmd *cmd)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			if (open_input_redir(redir) == -1)
				return (-1);
		}
		else if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (open_output_redir(redir) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

void	child_process(t_cmd *cmd, t_env **envp, int *pipe_fd, int fd_in)
{
	char	**env_array;

	init_child_signals();
	env_array = ft_env_list_to_array(*envp);
	if (fd_in != 0)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (child_handle_redirs(cmd) == -1)
		exit(1);
	child_execute_cmd(cmd, envp, env_array);
}
