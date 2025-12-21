/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:29:02 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 13:57:44 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_unclosed_quote(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		i++;
	}
	return (quote != 0);
}

/**
 * @brief process a single line of input
 * @note
 * 1. lexing: convert the input line into a list of tokens
 * 2. parsing: convert the list of tokens into an abstract syntax tree (AST)
 * 3. heredoc processing: handle any heredoc syntax in the AST
 * 4. execution: execute the commands represented by the AST
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
	ast = ft_parser(token_list);
	if (ast && ft_process_heredoc(ast, *env_list) == 0)
		ft_executor(ast, env_list);
	ft_free_tokens(&token_list);
	ft_free_ast(ast);
}

static void	ft_process_lines(char **lines, t_env **env_list)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		if (*lines[i] != '\0')
			ft_process_line(lines[i], env_list);
		i++;
	}
	ft_free_array(lines);
}

static void	ft_main_stream(char *line, char **lines, t_env **env_list)
{
	if (ft_strchr(line, '\n') && !has_unclosed_quote(line))
	{
		lines = ft_split(line, '\n');
		ft_process_lines(lines, env_list);
	}
	else
		ft_process_line(line, env_list);
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
	char	**lines;
	t_env	*env_list;

	(void)argc;
	(void)argv;
	line = NULL;
	lines = NULL;
	ft_init_signals();
	ft_disable_echo_ctl();
	env_list = ft_init_env(envp);
	while (1)
	{
		//line = readline("minishell$ ");
		line = ft_get_input();
		if (!line)
			break ;
		if (*line && isatty(STDIN_FILENO))
			add_history(line);
		ft_main_stream(line, lines, &env_list);
		free(line);
	}
	ft_free_env_list(env_list);
	return (g_signal);
}
