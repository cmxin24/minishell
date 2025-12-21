/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_7.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 23:00:31 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 23:06:35 by xin              ###   ########.fr       */
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
