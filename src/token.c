/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 16:23:05 by xin               #+#    #+#             */
/*   Updated: 2025/12/06 17:29:54 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(char *content, t_type_of_token type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = content;
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **token_list, t_token *new_token)
{
	t_token	*temp;

	if (!token_list || !new_token)
		return ;
	if (*token_list == NULL)
	{
		*token_list = new_token;
		return ;
	}
	temp = *token_list;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = new_token;
}

void	ft_free_tokens(t_token **token_list)
{
	t_token	*current;
	t_token	*next;

	if (!token_list)
		return ;
	current = *token_list;
	while (current)
	{
		next = current->next;
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
	*token_list = NULL;
}
