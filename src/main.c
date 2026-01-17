/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:29:02 by xin               #+#    #+#             */
/*   Updated: 2026/01/17 12:57:32 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief process a single line of input
 * @note
 * lexing: convert the input line into a list of tokens
 * parsing: convert the list of tokens into an abstract syntax tree (ast)
 * heredoc processing: handle heredoc syntax in the ast
 * execution: execute the commands represented by the ast
 */
static void	ft_process_line(char *line, t_env **env_list)
{
	t_ast	*ast;
	t_token	*token_list;

	if (!line || *line == '\0')
		return ;
	token_list = ft_lexer(line);
	if (!token_list)
		return ;
	ast = ft_parser(&token_list);
	if (ast && ft_process_heredoc(ast, *env_list) == 0)
		ft_executor(ast, env_list, 0);
	ft_free_tokens(&token_list);
	ft_free_ast(ast);
}

static void	ft_main_stream(char *line, t_env **env_list)
{
	int		i;
	char	**lines;

	lines = NULL;
	if (ft_strchr(line, '\n') && !has_unclosed_quote(line))
	{
		lines = ft_split_lines_safe(line);
		if (!lines)
			return ;
		i = 0;
		while (lines[i])
		{
			if (*lines[i] != '\0')
				ft_process_line(lines[i], env_list);
			if (!isatty(STDIN_FILENO) && g_signal == 2)
				break ;
			i++;
		}
		ft_free_array(lines);
	}
	else
		ft_process_line(line, env_list);
}

static char	*ft_read_until_quotes_closed(char *line)
{
	char	*next_line;
	char	*temp;

	while (has_unclosed_quote(line))
	{
		next_line = read_quote_line();
		if (!next_line)
		{
			ft_putstr_fd(
				"minishell: unexpected EOF while looking for matching quote\n",
				2);
			ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
			g_signal = 2;
			free(line);
			return (NULL);
		}
		temp = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(temp, next_line);
		free(temp);
		free(next_line);
	}
	return (line);
}

/**
 * @brief main function of minishell
 * @note
 * if user input EOF(Ctrl+D), then (!line), minishell will exit
 * isatty() check if the input is from terminal, only add history from terminal
 */
int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	*env_list;

	(void)argc;
	(void)argv;
	ft_init_signals();
	ft_disable_echo_ctl();
	env_list = ft_init_env(envp);
	while (1)
	{
		line = ft_get_input();
		if (!line)
			break ;
		line = ft_read_until_quotes_closed(line);
		if (!line)
			continue ;
		if (*line && isatty(STDIN_FILENO))
			add_history(line);
		ft_main_stream(line, &env_list);
		free(line);
		if (!isatty(STDIN_FILENO) && g_signal == 2)
			break ;
	}
	ft_free_env_list(env_list);
	return (rl_clear_history(), g_signal);
}
