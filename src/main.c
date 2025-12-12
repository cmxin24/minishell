/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:29:02 by xin               #+#    #+#             */
/*   Updated: 2025/12/12 16:36:49 by xin              ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*token_list;
	t_cmd	*cmd_list;
	t_env	*env_list;

	(void)argc;
	(void)argv;
	ft_init_signals();
	ft_disable_echo_ctl();
	env_list = ft_init_env(envp);
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			line = readline("minishell$ ");
		}
		else
		{
			char *gnl_line = get_next_line(STDIN_FILENO);
			if (gnl_line)
			{
				line = ft_strtrim(gnl_line, "\n");
				free(gnl_line);
			}
			else
				line = NULL;
		}
		//line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		token_list = ft_lexer(line);
		if (token_list == NULL)
		{
			free(line);
			continue ;
		}
		cmd_list = ft_parser(token_list);
		ft_expander(cmd_list, &env_list);
		if (cmd_list)
			ft_executor(cmd_list, &env_list);
		free(line);
		ft_free_tokens(&token_list);
		ft_free_cmd_list(cmd_list);
	}
	ft_free_env_list(env_list);
	return (0);
}
