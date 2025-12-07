/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 19:04:08 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 19:15:00 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_redirection(t_cmd *cmd, t_token **token)
{
	t_type_of_token	type;

	type = (*token)->type;
	*token = (*token)->next;
	if (!*token || (*token)->type != WORD)
	{
		if (!*token)
			syntax_error("newline");
		else
			syntax_error((*token)->content);
		return ;
	}
	if (type == REDIRECT_IN)
	{
		if (cmd->redirect_in)
			free(cmd->redirect_in);
		cmd->redirect_in = ft_strdup((*token)->content);
	}
	else if (type == REDIRECT_OUT || type == APPEND)
	{
		if (cmd->redirect_out)
			free(cmd->redirect_out);
		cmd->redirect_out = ft_strdup((*token)->content);
		if (type == APPEND)
			cmd->is_append = 1;
		else
			cmd->is_append = 0;
	}
	// TODO: HEREDOC
}
