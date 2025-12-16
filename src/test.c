/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:43:00 by meyu              #+#    #+#             */
/*   Updated: 2025/12/16 18:43:28 by meyu             ###   ########.fr       */
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
