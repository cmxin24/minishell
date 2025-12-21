/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 22:54:58 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 23:01:48 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	strip_handle_quote(t_strip_ctx *ctx)
{
	if ((ctx->str[ctx->i] == '\'' || ctx->str[ctx->i] == '\"')
		&& ctx->quote == 0)
	{
		ctx->quote = ctx->str[ctx->i];
		return (1);
	}
	if (ctx->str[ctx->i] == ctx->quote)
	{
		ctx->quote = 0;
		return (1);
	}
	return (0);
}

static void	strip_handle_char(t_strip_ctx *ctx)
{
	if (ctx->dst)
		ctx->dst[ctx->j] = ctx->str[ctx->i];
	ctx->j++;
}

static void	strip_quotes_run(t_strip_ctx *ctx)
{
	while (ctx->str[ctx->i])
	{
		if (strip_should_escape(ctx))
		{
			strip_handle_escape(ctx);
			ctx->i++;
			continue ;
		}
		if (strip_handle_quote(ctx))
		{
			ctx->i++;
			continue ;
		}
		strip_handle_char(ctx);
		ctx->i++;
	}
}

int	ft_len_without_quotes(char *str)
{
	t_strip_ctx	ctx;

	if (!str)
		return (0);
	ctx.str = str;
	ctx.i = 0;
	ctx.j = 0;
	ctx.quote = 0;
	ctx.dst = NULL;
	strip_quotes_run(&ctx);
	return (ctx.j);
}

char	*ft_strip_quotes(char *str, int len)
{
	t_strip_ctx	ctx;

	(void)len;
	if (!str)
		return (NULL);
	len = ft_len_without_quotes(str);
	ctx.dst = malloc(sizeof(char) * (len + 1));
	if (!ctx.dst)
		return (NULL);
	ctx.str = str;
	ctx.i = 0;
	ctx.j = 0;
	ctx.quote = 0;
	strip_quotes_run(&ctx);
	ctx.dst[ctx.j] = '\0';
	return (ctx.dst);
}
