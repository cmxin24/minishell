/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:52:22 by xin               #+#    #+#             */
/*   Updated: 2025/12/16 15:16:42 by meyu             ###   ########.fr       */
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

static int	heredoc_input_loop(char *delimiter, int fd, t_env *env, int quotes)
{
	char	*line;
	char	*expanded_line;
	char	*trimmed;

	while (1)
	{
		if (g_signal == 130)
			break ;
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document delimiter `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("' not found\n", 2);
			break ;
		}
		if (!isatty(STDIN_FILENO))
		{
			trimmed = ft_strtrim(line, "\n");
			free(line);
			line = trimmed;
		}
		if (quotes == 0)
		{
			expanded_line = expand_heredoc_line(line, env);
			if (ft_strcmp(expanded_line, delimiter) == 0)
			{
				free(line);
				free(expanded_line);
				break ;
			}
			free(line);
			line = expanded_line;
		}
		else
		{
			if (ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	return (0);
}

int	ft_heredoc(t_redir *redir, t_env *env)
{
	int		fd;
	char	*filename;
	char	*delimiter;
	int		quotes;

	delimiter = redir->file;
	quotes = redir->heredoc_quoted;
	filename = ft_heredoc_filename();
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (free(filename), -1);
	heredoc_input_loop(delimiter, fd, env, quotes);
	close(fd);
	free(redir->file);
	redir->file = filename;
	return (0);
}
