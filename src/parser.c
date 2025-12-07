/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:14:23 by xin               #+#    #+#             */
/*   Updated: 2025/12/06 17:30:03 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_cmd	*ft_new_cmd(void)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->content = NULL;
	new->next = NULL;
	return (new);
}

static int	ft_count_args(t_token *token)
{
	int	i;

	i = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD)
			i++;
		token = token->next;
	}
	return (i);
}

static t_cmd	*parse_single_command(t_token **temp)
{
	t_cmd	*cmd;
	int		arg_count;
	int		i;

	cmd = ft_new_cmd();
	if (!cmd)
		return (NULL);
	arg_count = ft_count_args(*temp);
	cmd->content = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->content)
		return (NULL);
	i = 0;
	while (*temp && (*temp)->type != PIPE)
	{
		if ((*temp)->type == WORD)
			cmd->content[i++] = ft_strdup((*temp)->content);
		*temp = (*temp)->next;
	}
	cmd->content[i] = NULL;
	return (cmd);
}

static void	ft_add_cmd(t_cmd **list, t_cmd **current, t_cmd *new_cmd)
{
	if (!*list)
	{
		*list = new_cmd;
		*current = new_cmd;
	}
	else
	{
		(*current)->next = new_cmd;
		*current = (*current)->next;
	}
}

t_cmd	*ft_parser(t_token *tokens)
{
	t_cmd	*list;
	t_cmd	*current;
	t_cmd	*new_node;
	t_token	*temp;

	list = NULL;
	current = NULL;
	temp = tokens;
	while (temp)
	{
		if (temp->type == PIPE)
		{
			temp = temp->next;
			continue ;
		}
		new_node = parse_single_command(&temp);
		if (!new_node)
		{
			ft_free_cmd_list(list);
			return (NULL);
		}
		ft_add_cmd(&list, &current, new_node);
	}
	return (list);
}
