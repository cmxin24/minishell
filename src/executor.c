/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:38:25 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 16:18:31 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_entry;
	char	*temp;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
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
	char	*path;
	char	**env_array;

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
	if (is_builtin(cmd->content[0]))
	{
		int exit_code = exec_builtin(cmd->content, envp);
		exit(exit_code);
	}
	path = find_command_path(cmd->content[0], env_array);
	if (!path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd->content[0]);
		exit(127);
	}
	execve(path, cmd->content, env_array);
	perror("execve");
	exit(1);
}

void	ft_executor(t_cmd *cmd_list, t_env **envp)
{
	t_cmd	*current;
	int		pipe_fd[2];
	int		fd_in;
	pid_t	pid;

	current = cmd_list;
	fd_in = 0;
	if (!current->next && is_builtin(current->content[0]))
	{
		exec_builtin(current->content, envp);
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
			child_process(current, envp, pipe_fd, fd_in);
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
	while (wait(NULL) > 0)
		;
}
