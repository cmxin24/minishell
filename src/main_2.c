/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:43:00 by meyu              #+#    #+#             */
/*   Updated: 2025/12/21 12:15:15 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	print_commands(t_cmd *cmd)
// {
// 	int	i;
// 	int	count;

// 	count = 1;
// 	printf("\n--- Command Structure ---\n");
// 	while (cmd)
// 	{
// 		printf("Command %d:\n", count++);
// 		if (cmd->content)
// 		{
// 			i = 0;
// 			while (cmd->content[i])
// 			{
// 				printf("  Arg[%d]: %s\n", i, cmd->content[i]);
// 				i++;
// 			}
// 		}
// 		cmd = cmd->next;
// 		if (cmd)
// 			printf("   |\n   v (Pipe)\n");
// 	}
// 	printf("-------------------------\n\n");
// }

char	*ft_get_input(void)
{
	char	*line;
	char	*gnl_line;

	if (isatty(STDIN_FILENO))
		line = readline("minishell$ ");
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

// char	*ft_handle_3000_tester(void)
// {
// 	char	*line;
// 	char	*continuation;
// 	char	*temp;
// 	char	*gnl_line;

// 	if (isatty(STDIN_FILENO))
// 	{
// 		line = readline("minishell$ ");
// 		if (line && has_unclosed_quote(line))
// 		{
// 			while (has_unclosed_quote(line))
// 			{
// 				continuation = readline("> ");
// 				if (!continuation)
// 					break ;
// 				temp = ft_strjoin(line, "\n");
// 				free(line);
// 				line = ft_strjoin(temp, continuation);
// 				free(temp);
// 				free(continuation);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		gnl_line = get_next_line(STDIN_FILENO);
// 		if (gnl_line)
// 		{
// 			line = ft_strtrim(gnl_line, "\n");
// 			free(gnl_line);
// 		}
// 		else
// 			line = NULL;
// 	}
// 	return (line);
// }
