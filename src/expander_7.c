/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_7.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meyu <meyu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:00:31 by xin               #+#    #+#             */
/*   Updated: 2026/01/17 15:30:53 by meyu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	strip_should_escape(t_strip_ctx *ctx)
{
	if (ctx->str[ctx->i] != '\\')
		return (0);
	if (ctx->quote == '\'')
		return (0);
	if (ctx->quote == '\"')
	{
		if (ctx->str[ctx->i + 1] == '$'
			|| ctx->str[ctx->i + 1] == '\"'
			|| ctx->str[ctx->i + 1] == '\\')
			return (1);
		return (0);
	}
	return (1);
}

void	strip_handle_escape(t_strip_ctx *ctx)
{
	ctx->i++;
	if (!ctx->str[ctx->i])
		return ;
	if (ctx->dst)
		ctx->dst[ctx->j] = ctx->str[ctx->i];
	ctx->j++;
}

void	free_list_nodes(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

char	**list_to_array(t_list *list)
{
	int		len;
	char	**array;
	int		i;
	t_list	*tmp;

	len = ft_lstsize(list);
	array = malloc(sizeof(char *) * (len + 1));
	if (!array)
		return (NULL);
	i = 0;
	tmp = list;
	while (tmp)
	{
		array[i++] = tmp->content;
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}
