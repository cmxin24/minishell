/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:38:25 by xin               #+#    #+#             */
/*   Updated: 2025/12/09 23:26:00 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				g_signal = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				g_signal = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					write(1, "Quit: 3\n", 8);
				else if (WTERMSIG(status) == SIGINT)
					write(1, "\n", 1);
			}
		}
	}
	ft_init_signals();
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_entry;
	char	*temp;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path_entry = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path_entry, X_OK) == 0)
			return (ft_free_array(paths), path_entry);
		free(path_entry);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

void	child_process(t_cmd *cmd, t_env **envp, int *pipe_fd, int fd_in)
{
	char		*path;
	char		**env_array;
	int			fd;
	int			exit_code;
	struct stat	st;
	t_redir		*redir;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	env_array = ft_env_list_to_array(*envp);
	if (fd_in != 0)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->next != NULL)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				ft_free_array(env_array);
				exit(1);
			}
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
				ft_free_array(env_array);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
	if (cmd->content == NULL || cmd->content[0] == NULL)
	{
		ft_free_array(env_array);
		exit(0);
	}
	if (is_builtin(cmd->content[0]))
	{
		exit_code = exec_builtin(cmd->content, envp);
		ft_free_array(env_array);
		exit(exit_code);
	}
	path = find_command_path(cmd->content[0], env_array);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->content[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->content[0], 2);
		ft_putstr_fd(": is a directory\n", 2);
		exit(126);
	}
	execve(path, cmd->content, env_array);
	ft_putstr_fd("minishell: ", 2);
	perror(cmd->content[0]);
	if (errno == EACCES)
		exit(126);
	if (errno == ENOENT)
		exit(127);
	exit(1);
}

void	ft_executor(t_cmd *cmd_list, t_env **env)
{
	t_cmd	*current;
	int		pipe_fd[2];
	int		fd_in;
	pid_t	pid;
	int		saved_stdout;
	int		saved_stdin;

	current = cmd_list;
	fd_in = 0;
	if (ft_process_heredoc(cmd_list, *env) == -1)
		return ;
	if (!current->next && is_builtin(current->content[0]))
	{
		if (ft_builtin_redirect(current, &saved_stdout, &saved_stdin) == 0)
		{
			g_signal = exec_builtin(current->content, env);
			ft_restore_io(saved_stdout, saved_stdin);
		}
		else
		{
			g_signal = 1;
			ft_restore_io(saved_stdout, saved_stdin);
		}
		return ;
	}
	while (current)
	{
		if (current->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		if (pid == 0)
			child_process(current, env, pipe_fd, fd_in);
		else
		{
			if (current->next)
				close(pipe_fd[1]);
			if (fd_in != 0)
				close(fd_in);
			if (current->next)
				fd_in = pipe_fd[0];
		}
		current = current->next;
	}
	ft_wait_for_children(pid);
}
