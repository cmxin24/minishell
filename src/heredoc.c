/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:52:22 by xin               #+#    #+#             */
/*   Updated: 2026/01/18 17:12:27 by nschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_heredoc_filename(void)
{
	static int	count = 0;
	char		*num;
	char		*filename;

	num = ft_itoa(count++);
	filename = ft_strjoin(".heredoc_tmp_", num);
	free(num);
	return (filename);
}

static char	*heredoc_read_line(void)
{
	char	*line;
	char	*trimmed;

	if (isatty(STDIN_FILENO))
		line = readline("> ");
	else
	{
		line = get_next_line(STDIN_FILENO);
		if (line)
		{
			trimmed = ft_strtrim(line, "\n");
			free(line);
			line = trimmed;
		}
	}
	return (line);
}

static void	heredoc_process_line(char *line, int fd, t_env *env, int quotes)
{
	char	*temp;

	if (quotes == 0)
	{
		temp = expand_heredoc_line(line, env);
		free(line);
		line = temp;
	}
	ft_putstr_fd(line, fd);
	ft_putstr_fd("\n", fd);
	free(line);
}

static int	heredoc_input_loop(char *delimiter, int fd, t_env *env, int quotes)
{
	char	*line;

	while (1)
	{
		line = heredoc_read_line();
		if (g_signal == 130)
			break ;
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimited b\
				y end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		heredoc_process_line(line, fd, env, quotes);
	}
	return (0);
}

int	ft_heredoc(t_redir *redir, t_env *env)
{
	int		fd;
	int		stdin_backup;
	char	*filename;
	int		result;

	filename = ft_heredoc_filename();
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (free(filename), -1);
	stdin_backup = dup(STDIN_FILENO);
	ft_set_heredoc_signals();
	result = heredoc_input_loop(redir->file, fd,
			env, redir->heredoc_quoted);
	ft_restore_signals();
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
	close(fd);
	if (g_signal == 130)
		return (unlink(filename), free(filename), -1);
	free(redir->file);
	return (redir->file = filename, result);
}
