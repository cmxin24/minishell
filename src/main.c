/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:29:02 by xin               #+#    #+#             */
/*   Updated: 2025/12/16 15:13:14 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_commands(t_cmd *cmd)
{
	int	i;
	int	count;

	count = 1;
	printf("\n--- Command Structure ---\n");
	while (cmd)
	{
		printf("Command %d:\n", count++);
		if (cmd->content)
		{
			i = 0;
			while (cmd->content[i])
			{
				printf("  Arg[%d]: %s\n", i, cmd->content[i]);
				i++;
			}
		}
		cmd = cmd->next;
		if (cmd)
			printf("   |\n   v (Pipe)\n");
	}
	printf("-------------------------\n\n");
}

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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**lines;
	t_token	*token_list;
	t_cmd	*cmd_list;
	t_env	*env_list;
	int		i;

	(void)argc;
	(void)argv;
	ft_init_signals();
	ft_disable_echo_ctl();
	env_list = ft_init_env(envp);
	while (1)
	{
		// if (isatty(STDIN_FILENO))
		// {
		// 	line = readline("minishell$ ");
		// 	if (line && has_unclosed_quote(line))
		// 	{
		// 		char *continuation;
		// 		char *temp;
		// 		while (has_unclosed_quote(line))
		// 		{
		// 			continuation = readline("> ");
		// 			if (!continuation)
		// 				break;
		// 			temp = ft_strjoin(line, "\n");
		// 			free(line);
		// 			line = ft_strjoin(temp, continuation);
		// 			free(temp);
		// 			free(continuation);
		// 		}
		// 	}
		// }
		// else
		// {
		// 	char *gnl_line = get_next_line(STDIN_FILENO);
		// 	if (gnl_line)
		// 	{
		// 		line = ft_strtrim(gnl_line, "\n");
		// 		free(gnl_line);
		// 	}
		// 	else
		// 		line = NULL;
		// }
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (ft_strchr(line, '\n') && !has_unclosed_quote(line))
		{
			lines = ft_split(line, '\n');
			i = 0;
			while (lines && lines[i])
			{
				if (*lines[i] == '\0')
				{
					i++;
					continue ;
				}
				token_list = ft_lexer(lines[i]);
				if (token_list == NULL)
				{
					i++;
					continue ;
				}
				cmd_list = ft_parser(token_list);
				ft_expander(cmd_list, &env_list);
				if (cmd_list)
					ft_executor(cmd_list, &env_list);
				ft_free_tokens(&token_list);
				ft_free_cmd_list(cmd_list);
				i++;
			}
			ft_free_array(lines);
		}
		else
		{
			token_list = ft_lexer(line);
			if (token_list != NULL)
			{
				cmd_list = ft_parser(token_list);
				ft_expander(cmd_list, &env_list);
				if (cmd_list)
					ft_executor(cmd_list, &env_list);
				ft_free_tokens(&token_list);
				ft_free_cmd_list(cmd_list);
			}
		}
		free(line);
	}
	ft_free_env_list(env_list);
	return (g_signal);
}
