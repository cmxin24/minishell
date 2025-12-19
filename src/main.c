/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:29:02 by xin               #+#    #+#             */
/*   Updated: 2025/12/19 12:21:44 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	has_unclosed_quote(char *str)
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

static char	*ft_handle_3000_tester(void)
{
	char	*line;
	char	*continuation;
	char	*temp;
	char	*gnl_line;

	if (isatty(STDIN_FILENO))
	{
		line = readline("minishell$ ");
		if (line && has_unclosed_quote(line))
		{
			while (has_unclosed_quote(line))
			{
				continuation = readline("> ");
				if (!continuation)
					break ;
				temp = ft_strjoin(line, "\n");
				free(line);
				line = ft_strjoin(temp, continuation);
				free(temp);
				free(continuation);
			}
		}
	}
	else
	{
		gnl_line = get_next_line(STDIN_FILENO);
		if (gnl_line)
		{
			line = ft_strtrim(gnl_line, "\n");
			free(gnl_line);
		}
		else
			line = NULL;
	}
	return (line);
}

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
	//ft_expander(ast, env_list);
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

static void	ft_main_steam(char *line, char **lines, t_env **env_list)
{
	if (ft_strchr(line, '\n') && !has_unclosed_quote(line))
	{
		lines = ft_split(line, '\n');
		ft_process_lines(lines, env_list);
	}
	else
		ft_process_line(line, env_list);
}

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
		line = ft_handle_3000_tester();
		//line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line && isatty(STDIN_FILENO))
			add_history(line);
		ft_main_steam(line, lines, &env_list);
		free(line);
	}
	ft_free_env_list(env_list);
	return (g_signal);
}
