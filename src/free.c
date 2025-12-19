/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:29:55 by xin               #+#    #+#             */
/*   Updated: 2025/12/19 12:10:42 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_free_ast(t_ast *ast);

void	ft_free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;
	t_redir	*redir;
	t_redir	*tmp_redir;

	while (cmd)
	{
		tmp = cmd->next;
		ft_free_array(cmd->content);
		if (cmd->subshell)
			ft_free_ast(cmd->subshell);
		redir = cmd->redirs;
		while (redir)
		{
			tmp_redir = redir->next;
			if (redir->type == REDIR_HEREDOC && redir->file)
				unlink(redir->file);
			if (redir->file)
				free(redir->file);
			free(redir);
			redir = tmp_redir;
		}
		free(cmd);
		cmd = tmp;
	}
}

void	ft_free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->type == AST_PIPELINE)
		ft_free_cmd_list(ast->pipeline);
	else
	{
		ft_free_ast(ast->left);
		ft_free_ast(ast->right);
	}
	free(ast);
}

void	ft_free_env_list(t_env *env_list)
{
	t_env	*temp;

	while (env_list)
	{
		temp = env_list->next;
		free(env_list->key);
		free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}
