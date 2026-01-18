/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:43:18 by xin               #+#    #+#             */
/*   Updated: 2026/01/18 16:57:46 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	open_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->file);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	open_output_redir(t_redir *redir)
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
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static void	handle_last_pid_status(pid_t pid, pid_t last_pid, int status)
{
	if (pid != last_pid)
		return ;
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

void	ft_wait_for_children(pid_t last_pid)
{
	int					status;
	pid_t				pid;
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		handle_last_pid_status(pid, last_pid, status);
	}
	ft_init_signals();
}

char	*find_command_path(char *cmd, char **envp, int i, char *temp)
{
	char	**paths;
	char	*path_entry;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	else
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
	return (ft_free_array(paths), NULL);
}
